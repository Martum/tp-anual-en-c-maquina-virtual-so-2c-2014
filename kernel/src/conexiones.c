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

// Lista y mutex para conexiones sin asignar (recien recibidas)
pthread_mutex_t mutex_conexiones_unsigned = PTHREAD_MUTEX_INITIALIZER;
t_list* conexiones_unasigned;

// Lista y mutex para conexiones de procesos
pthread_mutex_t mutex_conexiones_procesos = PTHREAD_MUTEX_INITIALIZER;
t_list* conexiones_procesos;

// SET para conexiones unasigned y de procesos
fd_set readfds_procesos;

//---------------------

// Lista y mutex para conexiones cpu
pthread_mutex_t mutex_conexiones_cpu = PTHREAD_MUTEX_INITIALIZER;
t_list* conexiones_cpu;

// SET para conexiones cpu
fd_set readfds_cpus;

// Socket y mutex de la conexion con memoria
pthread_mutex_t mutex_conexion_memoria = PTHREAD_MUTEX_INITIALIZER;
sock_t* conexion_memoria;

void _agregar_conexion_a_unasigned(sock_t* conexion)
{
	pthread_mutex_lock(&mutex_conexiones_unsigned);

	list_add(conexiones_unasigned, conexion);
	FD_SET(conexion->fd, &readfds_procesos);

	pthread_mutex_unlock(&mutex_conexiones_unsigned);
}

void _agregar_conexion_a_procesos(sock_t* conexion, uint32_t pid)
{
	conexion_proceso_t* conexion_proceso = malloc(sizeof(conexion_proceso_t));
	conexion_proceso->pid = pid;
	conexion_proceso->socket = conexion;

	pthread_mutex_lock(&mutex_conexiones_procesos);

	list_add(conexiones_procesos, conexion_proceso);
	FD_SET(conexion->fd, &readfds_procesos);

	pthread_mutex_unlock(&mutex_conexiones_procesos);
}

void _eliminar_conexion_de_unasigned(int32_t fd)
{
	bool buscar_unasigned(void* elemento)
	{
		sock_t* conexion = (sock_t*) elemento;

		return conexion->fd == fd;
	}

	pthread_mutex_lock(&mutex_conexiones_unsigned);

	list_remove_by_condition(conexiones_unasigned, buscar_unasigned);

	pthread_mutex_unlock(&mutex_conexiones_unsigned);
}

void _agregar_conexion_a_cpu(sock_t* conexion)
{
	pthread_mutex_lock(&mutex_conexiones_cpu);
	//TODO: Agregar un struct para wrappear las conexiones a los cpus con un ID
	list_add(conexiones_cpu, conexion);
	FD_SET(conexion->fd, &readfds_cpus);

	pthread_mutex_unlock(&mutex_conexiones_cpu);
}

void _recalcular_mayor_fd(int32_t* mayor_fd, int32_t nuevo_fd)
{
	if(*mayor_fd < nuevo_fd)
		*mayor_fd = nuevo_fd;
}

void _dar_bienvenida(sock_t* nueva_conexion)
{
	// Respondemos con BIENVENIDA
	char* respuesta = malloc(tamanio_flagt());
	flag_t resp = BIENVENIDO;
	uint32_t i = tamanio_flagt();

	memcpy(respuesta, &resp, tamanio_flagt());

	enviar(nueva_conexion, respuesta, &i);

	free(respuesta);

}

/**
 * Se procesa un nuevo programa.
 * Tenemos que llamar a los metodos del loader.h y agregar el fd en donde corresponda
 */
void _procesar_conexion_nuevo_programa(char* codigo_beso, uint32_t longitud, sock_t* conexion)
{
	// Recordar agregar el fd al set master de procesos, y el registro a la lista
	int32_t pid = procesar_nuevo_programa(codigo_beso, longitud);

	// Si no se pudo alocar memoria, notificamos al proceso
	if(pid == -1)
	{	// TODO: HACER ESTA FUNCION
		_informar_no_hay_memoria(conexion);
		return;
	}

	// Agregamos la conexion a la lista de procesos
	_agregar_conexion_a_procesos(conexion, pid);
}

/**
 * Procesa un nueva conexion entrante
 *
 * @RETURNS: Si es un PROGRAMA devuelve 1; si es un CPU devuelve 2.
 * 			Deja en nueva_conexion el socket
 */
int32_t _procesar_nueva_conexion(sock_t* principal, sock_t* nueva_conexion)
{
	// Aceptamos conexion
	nueva_conexion = aceptar_conexion(principal);
	uint32_t i;
	char* msg;

	// Recibimos la identeificacion de la conexion
	recibir(nueva_conexion, &msg, &i);
	flag_t codop = codigo_operacion(msg);

	int salida = 0;

	switch(codop)
	{
		case SOY_PROGRAMA:
			_dar_bienvenida(nueva_conexion);

			// Recibimos el codigo BESO del programa
			char* codigo_beso;
			uint32_t len;

			if(recibir(nueva_conexion, &codigo_beso, &len) == -1)
			{// Si el mensaje no se recibe completo
				_informar_mensaje_incompleto(nueva_conexion->fd);
				free(codigo_beso);
				break;
			}

			// Procesamos el nuevo programa
			_procesar_conexion_nuevo_programa(codigo_beso, len, nueva_conexion);

			free(codigo_beso);
			salida = 1;
			break;

		case SOY_CPU:
			//TODO: Progamar el comportamiento al recibir la conexion de un CPU
			_dar_bienvenida(nueva_conexion);

			salida = 2;
			break;

		default:
			break;
	}

	free(msg);
	return salida;
}

// Atiende las conexiones de procesos y de conexiones que todavia no se asignaron
void _atender_socket_proceso(int32_t fd)
{
	// Variables para el mensaje
	// IMPORTANTE: NO LIBERAR MENSAJE DENTRO DE UNA FUNCION,
	// SE LIBERA AL FINAL DEL SWITCH
	char* mensaje;
	uint32_t len;

	// Creamos un socket fantasma con el fd
	sock_t socket_fantasma;
	socket_fantasma.fd = fd;

	// Recibimos el mensaje y obtenemos el codigo operacion
	int32_t resultado = recibir(&socket_fantasma, &mensaje, &len);
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
				_informar_mensaje_incompleto(fd);
				break;
		}
	}
	else
		_informar_mensaje_incompleto(fd);

	// Liberamos el buffer
	free(mensaje);

}

void _atender_socket_cpu(int32_t fd)
{
	//TODO: Atendeme el socket maestro
}

// Corre en un THREAD
void* escuchar_conexiones_entrantes_y_procesos(void* un_ente)
{
	sock_t* principal = crear_socket_escuchador(puerto());
	escuchar(principal);

	// Seteamos este como el socket mas grande
	int32_t mayor_fd = principal->fd;
	FD_ZERO(&readfds_procesos);
	FD_SET(principal->fd, &readfds_procesos);

	// Preparamos el SET
	fd_set readfds = readfds_procesos;

	while(1)
	{
		// Escuchamos el universo
		int rs = select(mayor_fd+1, &readfds, NULL, NULL, NULL);

		if(rs != -1)
		{
			// Vemos si hay sockets para leer
			int32_t i;
			int32_t copia_mayor_fd = mayor_fd;
			for(i = 0; i < copia_mayor_fd; i++)
			{
				// Si el socket se puede leer
				if(FD_ISSET(i, &readfds))
				{

					if(i == principal->fd)
					{// Es el socket principal, new connection knocking
						sock_t* nueva_conexion;

						if(_procesar_nueva_conexion(principal, nueva_conexion) == 1)
							_recalcular_mayor_fd(&mayor_fd, nueva_conexion->fd);
					}
					else
					{// No es el socket principal, es un proceso
						_atender_socket_proceso(i);
					}
				}
			}
		}

		// Rearmamos el readfds
		readfds = readfds_procesos;
	}

	return NULL;
}

// Corre en un THREAD
void* escuchar_cpus(void* otro_ente)
{
	// TODO: SIMILAR A LA SUPERIOR PERO PARA ESCUCHAR CPUS
	// ESTE DEBE TENER UN TIMER QUE CADA X SEGUNDOS REARME EL FDS
}

void inicializar_listas_conexiones(void)
{
	conexiones_unasigned = list_create();
	//pthread_mutex_init(&mutex_conexiones_unsigned, NULL);

	conexiones_cpu = list_create();
	//pthread_mutex_init(&mutex_conexiones_cpu, NULL);

	conexiones_procesos = list_create();
	//pthread_mutex_init(&mutex_conexiones_procesos, NULL);

	//pthread_mutex_init(&mutex_conexion_memoria, NULL);
}

sock_t* buscar_conexion_proceso_por_pid(uint32_t pid)
{
	// Funcion de busqueda
	bool buscar_proceso(void* elemento)
	{
		conexion_proceso_t* conexion = (conexion_proceso_t*) elemento;

		return conexion->pid == pid;
	}

	// Buscamos...
	pthread_mutex_lock(&mutex_conexiones_procesos);
	conexion_proceso_t* conexion = list_find(conexiones_procesos, buscar_proceso);
	pthread_mutex_unlock(&mutex_conexiones_procesos);

	return conexion->socket;
}

sock_t* buscar_conexion_proceso_por_fd(int32_t fd)
{
	// Funcion de busqueda
	bool buscar_proceso(void* elemento)
	{
		conexion_proceso_t* conexion = (conexion_proceso_t*) elemento;

		return conexion->socket->fd == fd;
	}

	// Buscamos...
	pthread_mutex_lock(&mutex_conexiones_procesos);
	conexion_proceso_t* conexion = list_find(conexiones_procesos, buscar_proceso);
	pthread_mutex_unlock(&mutex_conexiones_procesos);

	return conexion->socket;
}

sock_t* buscar_conexion_unasigned_por_fd(int32_t fd)
{
	// Funcion de busqueda
	bool buscar_unasigned(void* elemento)
	{
		sock_t* conexion = (sock_t*) elemento;

		return conexion->fd == fd;
	}

	// Buscamos...
	pthread_mutex_lock(&mutex_conexiones_unsigned);
	sock_t* conexion = list_find(conexiones_unasigned, buscar_unasigned);
	pthread_mutex_unlock(&mutex_conexiones_unsigned);

	return conexion;
}
