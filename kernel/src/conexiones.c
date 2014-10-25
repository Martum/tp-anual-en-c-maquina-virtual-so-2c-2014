/*
 * conexiones.c
 *
 *  Created on: 22/9/2014
 *      Author: martin
 */

#include <stdio.h>
#include <stdlib.h>

#include <commons/collections/list.h>
#include <hu4sockets/sockets.h>
#include <stdbool.h>
#include <pthread.h>
#include "configuraciones.h"
#include "conexiones.h"
#include <hu4sockets/mensajes.h>
#include "loader.h"
#include <sys/time.h>
#include "cpu.h"

// Lista y mutex para conexiones de procesos
pthread_mutex_t MUTEX_CONEXIONES_PROCESOS = PTHREAD_MUTEX_INITIALIZER;
t_list* CONEXIONES_PROCESOS;

// SET para conexiones unasigned y de procesos
fd_set READFDS_PROCESOS;

//---------------------

// Lista y mutex para conexiones cpu
pthread_mutex_t MUTEX_CONEXIONES_CPU = PTHREAD_MUTEX_INITIALIZER;
t_list* CONEXIONES_CPU;

// SET para conexiones cpu
fd_set READFDS_CPUS;

int32_t MAYOR_FD_CPU = -1;

//--------------------

// Socket y mutex de la conexion con memoria
pthread_mutex_t MUTEX_CONEXION_MEMORIA = PTHREAD_MUTEX_INITIALIZER;
sock_t* CONEXION_MEMORIA;

/**
 * Agrega una conexion a la lista de conexiones de Procesos
 */
void _agregar_conexion_a_procesos(sock_t* conexion, uint32_t pid)
{
	conexion_proceso_t* conexion_proceso = malloc(sizeof(conexion_proceso_t));
	conexion_proceso->pid = pid;
	conexion_proceso->socket = conexion;

	pthread_mutex_lock(&MUTEX_CONEXIONES_PROCESOS);

	list_add(CONEXIONES_PROCESOS, conexion_proceso);
	FD_SET(conexion->fd, &READFDS_PROCESOS);

	pthread_mutex_unlock(&MUTEX_CONEXIONES_PROCESOS);
}

/**
 * Agrega una conexion a la lista de conexiones de CPU
 */
void _agregar_conexion_a_cpu(sock_t* conexion, uint32_t id)
{
	conexion_cpu_t* conexion_cpu = malloc(sizeof(conexion_cpu_t));
	conexion_cpu->socket = conexion;
	conexion_cpu->id = id;

	pthread_mutex_lock(&MUTEX_CONEXIONES_CPU);

	list_add(CONEXIONES_CPU, conexion_cpu);
	FD_SET(conexion->fd, &READFDS_CPUS);

	pthread_mutex_unlock(&MUTEX_CONEXIONES_CPU);
}

void _recalcular_mayor_fd(int32_t* mayor_fd, int32_t nuevo_fd)
{
	if(*mayor_fd < nuevo_fd)
		*mayor_fd = nuevo_fd;
}

void _enviar_flagt(sock_t* conxion, flag_t flag)
{
	char* msg = malloc(tamanio_flagt());
	uint32_t i = tamanio_flagt();

	memcpy(msg, &flag, tamanio_flagt());

	enviar(conxion, msg, &i);

	free(msg);
}

/**
 * Se da la bienvenida al Proceso o al CPU
 */
void _dar_bienvenida(sock_t* nueva_conexion)
{
	// Respondemos con BIENVENIDA
	_enviar_flagt(nueva_conexion, BIENVENIDO);
}

/**
 * Se comunica al proceso que se recibio OK el mensaje
 */
void _enviar_ok(sock_t* conexion)
{
	_enviar_flagt(conexion, RECIBIDO_OK);

}

/**
 * Informa al destinatario que el mensaje no se recibio correctamente
 * Usar solo si el destinatario esta esperando una confirmacion de recepcion
 */
void _informar_mensaje_incompleto(sock_t* conexion)
{
	_enviar_flagt(conexion, MENSAJE_INCOMPLETO);
}

/**
 * Se informa al Proceso que no hay memoria para iniciarlo
 */
void _informar_no_hay_memoria(sock_t* conexion)
{
	_enviar_flagt(conexion, SIN_MEMORIA);
}

/**
 * Se procesa un nuevo programa.
 * Tenemos que llamar a los metodos del loader.h y agregar el fd en donde corresponda
 * @RETURNS: -1 en caso de no haber memoria suficiente; 0 en caso de exito
 */
int32_t _procesar_conexion_nuevo_programa(char* codigo_beso, uint32_t longitud, sock_t* conexion)
{
	// Recordar agregar el fd al set master de procesos, y el registro a la lista
	int32_t pid = procesar_nuevo_programa(codigo_beso, longitud);

	// Si no se pudo alocar memoria, notificamos al proceso
	if(pid == -1)
		return -1;

	// Agregamos la conexion a la lista de procesos
	_agregar_conexion_a_procesos(conexion, pid);

	return 0;
}

/**
 * Procesa un nueva conexion entrante
 *
 * @RETURNS: Si es un PROGRAMA devuelve 1; si es un CPU devuelve 2; si no se
 * 			acepta la conexion devuelve -1
 * 			Deja en nueva_conexion el socket
 */
int32_t _procesar_nueva_conexion(sock_t* principal, sock_t** nueva_conexion)
{
	// Aceptamos conexion
	*nueva_conexion = aceptar_conexion(principal);
	uint32_t i;
	char* msg;

	// Recibimos la identeificacion de la conexion
	recibir(*nueva_conexion, &msg, &i);
	flag_t codop = codigo_operacion(msg);

	int32_t salida = -1;

	switch(codop)
	{
		case SOY_PROGRAMA:
			salida = 1;
			_dar_bienvenida(*nueva_conexion);

			// Recibimos el codigo BESO del programa
			char* codigo_beso;
			uint32_t len;

			if(recibir(*nueva_conexion, &codigo_beso, &len) == -1)
			{// Si el mensaje no se recibe completo
				_informar_mensaje_incompleto(buscar_conexion_proceso_por_fd((*nueva_conexion)->fd));
				free(codigo_beso);
				break;
			}

			// Procesamos el nuevo programa
			if(_procesar_conexion_nuevo_programa(codigo_beso, len, *nueva_conexion) == -1)
			{// Si no hay memoria, informamos y seteamos salida con -1
				_informar_no_hay_memoria(*nueva_conexion);
				salida = -1;
			}
			else
			{// Esta OK
				_enviar_ok(*nueva_conexion);
			}

			free(codigo_beso);
			break;

		case SOY_CPU:
			salida = 2;

			// Agregamos el CPU a la lista
			_agregar_conexion_a_cpu(*nueva_conexion, dame_nuevo_id_cpu());

			// Le damos la bienvenida
			_dar_bienvenida(*nueva_conexion);
			break;

		default:
			break;
	}

	free(msg);
	return salida;
}

// Atiende las conexiones de procesos y de conexiones que todavia no se asignaron
void _atender_socket_proceso(conexion_proceso_t* conexion_proceso)
{
	// Variables para el mensaje
	// IMPORTANTE: NO LIBERAR MENSAJE DENTRO DE UNA FUNCION,
	// SE LIBERA AL FINAL DEL SWITCH
	char* mensaje;
	uint32_t len;

	// Recibimos el mensaje y obtenemos el codigo operacion
	int32_t resultado = recibir(conexion_proceso->socket, &mensaje, &len);
	flag_t cod_op = codigo_operacion(mensaje);

	if(resultado == 0)
	{// Se recibio la totalidad de los bytes

		// Creamos copia del puntero al mensaje y a su len
		// Hay que usar estos dentro de las funciones,
		// ya que tienen desplazado el mensaje
		char* copia_mensaje = mensaje + tamanio_flagt();
		uint32_t copia_len = len - tamanio_flagt();

		// Vemos que operacion es
		switch(cod_op)
		{
	/*		case bla bla
				foo(copia_mensaje(chorrobytes), copia_len(longitud del chorro))
				break;*/

			default:
				_informar_mensaje_incompleto(conexion_proceso->socket);
				break;
		}
	}
	else
		_informar_mensaje_incompleto(conexion_proceso->socket);

	// Liberamos el buffer
	free(mensaje);

}

void _atender_socket_cpu(conexion_cpu_t* conexion_cpu)
{
	//TODO: Atendeme el socket maestro
	char* mensaje;
	uint32_t len;

	int32_t resultado = recibir(conexion_cpu->socket, &mensaje, &len);
	flag_t cod_op = codigo_operacion(mensaje);

	if(resultado == 0)
	{
		switch (cod_op) {
			case SALIDA_ESTANDAR:
				;
				pedido_salida_estandar_t* pedido_salida = deserializar_pedido_salida_estandar_t(mensaje);

				salida_estandar(pedido_salida);

				free(pedido_salida->cadena_de_texto);
				free(pedido_salida);
				break;

			case ENTRADA_ESTANDAR:
				;
				pedido_entrada_estandar_t* pedido_entrada = deserializar_pedido_entrada_estandar_t(mensaje);

				enviar_entrada_estandar(pedido_entrada);

				free(pedido_entrada);
				break;


			default:
				break;
		}
	}

	free(mensaje);
}

// Corre en un THREAD
void* escuchar_conexiones_entrantes_y_procesos(void* un_ente)
{
	sock_t* principal = crear_socket_escuchador(puerto());
	escuchar(principal);

	// Seteamos el timer
	struct timeval timer;
	timer.tv_sec = 5;
	timer.tv_usec = 0;

	// Seteamos este como el socket mas grande
	int32_t mayor_fd = principal->fd;
	FD_ZERO(&READFDS_PROCESOS);
	FD_SET(principal->fd, &READFDS_PROCESOS);

	// Preparamos el SET
	fd_set readfds = READFDS_PROCESOS;

	while(1)
	{
		// Escuchamos el universo
		int rs = select(mayor_fd+1, &readfds, NULL, NULL, &timer);

		if(rs > 0)
		{// Podemos leer
			// Vemos si hay sockets para leer
			int32_t i;
			int32_t copia_mayor_fd = mayor_fd;
			for(i = 0; i < (copia_mayor_fd+1); i++)
			{
				// Si el socket se puede leer
				if(FD_ISSET(i, &readfds))
				{

					if(i == principal->fd)
					{// Es el socket principal, new connection knocking
						sock_t* nueva_conexion = NULL;
						if(_procesar_nueva_conexion(principal, &nueva_conexion) == 1)
						{// Es programa y salio all ok
							_recalcular_mayor_fd(&mayor_fd, nueva_conexion->fd);
						}
						else
						{// Es CPU
							//TODO: Aca el quehacer cuando la conexion es CPU
							_recalcular_mayor_fd(&MAYOR_FD_CPU, nueva_conexion->fd);
						}
					}
					else
					{// No es el socket principal, es un proceso
						_atender_socket_proceso(buscar_proceso_por_fd(i));
					}
				}
			}
		}

		// Rearmamos el readfds
		readfds = READFDS_PROCESOS;

		// Reseteamos el timer
		timer.tv_sec = 5;
		timer.tv_usec = 0;
	}

	return NULL;
}

// Corre en un THREAD
void* escuchar_cpus(void* otro_ente)
{
	// Seteamos el timer
	struct timeval timer;
	timer.tv_sec = 5;
	timer.tv_usec = 0;

	// Inicializamos el readfds maestro y el mayor
	int32_t mayor_fd = MAYOR_FD_CPU;
	FD_ZERO(&READFDS_CPUS);

	// Seteamos el readfds
	fd_set readfds = READFDS_CPUS;

	while(1)
	{
		int rs = select(mayor_fd+1, &readfds, NULL, NULL, &timer);

		if(rs > 0)
		{// Podemos leer
			// Vemos si hay sockets para leer
			int32_t i;
			int32_t copia_mayor_fd = mayor_fd;
			for(i = 0; i < (copia_mayor_fd+1); i++)
			{
				if(FD_ISSET(i, &readfds))
				{// Atendemos al socket
					_atender_socket_cpu(buscar_cpu_por_fd(i));
				}
			}
		}

		// Reseteamos el timer
		timer.tv_sec = 5;
		timer.tv_usec = 0;

		// Rearmamos el readfds
		readfds = READFDS_CPUS;

		// Seteamos el nuevo mayor (si es que hay)
		mayor_fd = MAYOR_FD_CPU;
	}

	return NULL;
}

void inicializar_listas_conexiones(void)
{

	CONEXIONES_CPU = list_create();
	//pthread_mutex_init(&mutex_conexiones_cpu, NULL);

	CONEXIONES_PROCESOS = list_create();
	//pthread_mutex_init(&mutex_conexiones_procesos, NULL);

	//pthread_mutex_init(&mutex_conexion_memoria, NULL);
}


// ********* BUSQUEDA DE PROCESOS ***************

sock_t* buscar_conexion_proceso_por_pid(uint32_t pid)
{
	// Funcion de busqueda
	bool buscar_proceso(void* elemento)
	{
		conexion_proceso_t* conexion = (conexion_proceso_t*) elemento;

		return conexion->pid == pid;
	}

	// Buscamos...
	pthread_mutex_lock(&MUTEX_CONEXIONES_PROCESOS);
	conexion_proceso_t* conexion = list_find(CONEXIONES_PROCESOS, buscar_proceso);
	pthread_mutex_unlock(&MUTEX_CONEXIONES_PROCESOS);

	return conexion->socket;
}

conexion_proceso_t* buscar_proceso_por_fd(int32_t fd)
{
	bool buscar_proceso(void* elemento)
	{
		conexion_proceso_t* conexion = (conexion_proceso_t*) elemento;

		return conexion->socket->fd == fd;
	}

	// Buscamos...
	pthread_mutex_lock(&MUTEX_CONEXIONES_PROCESOS);
	conexion_proceso_t* conexion = list_find(CONEXIONES_PROCESOS, buscar_proceso);
	pthread_mutex_unlock(&MUTEX_CONEXIONES_PROCESOS);

	return conexion;
}

sock_t* buscar_conexion_proceso_por_fd(int32_t fd)
{
	return buscar_proceso_por_fd(fd)->socket;
}


// ********* BUSQUEDA DE CPUs ***************

sock_t* buscar_conexion_cpu_por_id(uint32_t id)
{
	// Funcion de busqueda
	bool buscar_cpu(void* elemento)
	{
		conexion_cpu_t* conexion = (conexion_cpu_t*) elemento;

		return conexion->id == id;
	}

	// Buscamos...
	pthread_mutex_lock(&MUTEX_CONEXIONES_CPU);
	conexion_proceso_t* conexion = list_find(CONEXIONES_CPU, buscar_cpu);
	pthread_mutex_unlock(&MUTEX_CONEXIONES_CPU);

	return conexion->socket;
}

conexion_cpu_t* buscar_cpu_por_fd(int32_t fd)
{
	bool buscar_cpu(void* elemento)
	{
		conexion_cpu_t* conexion = (conexion_cpu_t*) elemento;

		return conexion->socket->fd == fd;
	}

	// Buscamos...
	pthread_mutex_lock(&MUTEX_CONEXIONES_CPU);
	conexion_cpu_t* conexion = list_find(CONEXIONES_CPU, buscar_cpu);
	pthread_mutex_unlock(&MUTEX_CONEXIONES_CPU);

	return conexion;
}

sock_t* buscar_conexion_cpu_por_fd(int32_t fd)
{
	return buscar_cpu_por_fd(fd)->socket;
}
