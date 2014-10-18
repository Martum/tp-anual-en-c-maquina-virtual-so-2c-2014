
#include <stdlib.h>
#include <stdio.h>

#include "configuraciones.h"
#include "conexiones_cpu.h"

#include <commons/collections/list.h>

#include <hu4sockets/mensajes.h>
#include <hu4sockets/sockets.h>

#include <pthread.h>


pthread_mutex_t mutex_conexiones_cpus = PTHREAD_MUTEX_INITIALIZER;
t_list* lista_conexiones_cpu;
fd_set readfds_cpus;

void inicializar_lista_conexiones_cpu(){
	lista_conexiones_cpu = list_create();
}

void _agregar_conexion_a_cpu(sock_t* conexion){
	pthread_mutex_lock(&mutex_conexiones_cpus);
	//TODO: Agregar un struct para wrappear las conexiones a los cpus con un ID
	list_add(lista_conexiones_cpu, conexion);
	FD_SET(conexion->fd, &readfds_cpus);

	pthread_mutex_unlock(&mutex_conexiones_cpus);
}

void _enviar_flagt(sock_t* conexion, flag_t flag){
	char* respuesta = malloc(tamanio_flagt());
	flag_t resp = flag;
	uint32_t i = tamanio_flagt();

	memcpy(respuesta, &resp, tamanio_flagt());

	enviar(conexion, respuesta, &i);

	free(respuesta);
}

void _informar_te_doy_segmento(sock_t* conexion){
	_enviar_flagt(conexion, TOMA_SEGMENTO);
}

void _informar_respuesta_destruccion_segmento(sock_t* conexion){
	_enviar_flagt(conexion, RESPUESTA_DESTRUCCION);
}

void _informar_te_doy_bytes(sock_t* conexion){
	_enviar_flagt(conexion, TOMA_BYTES);
}

void _informar_respuesta_escritura(sock_t* conexion){
	// NO SE PORQUE NO ME COMPILA ESTE FLAG
	// _enviar_flagt(conexion, RESPUESTA_ESCRITURA);
}


sock_t* buscar_conexion_cpu_por_fd(int32_t fd){
	// Funcion de busqueda
	bool buscar_cpu(void* elemento){
		conexion_cpu_t* conexion = (conexion_cpu_t*) elemento;
		return (conexion->socket->fd) == fd;
	}

	// Buscamos...
	pthread_mutex_lock(&mutex_conexiones_cpus);
	conexion_cpu_t* conexion = list_find(lista_conexiones_cpu, buscar_cpu);
	pthread_mutex_unlock(&mutex_conexiones_cpus);

	return conexion->socket;
}

void* escuchar_cpus(void* otro_ente){

	sock_t* principal = crear_socket_escuchador(puerto());
	escuchar(principal);

	// Seteamos este como el socket mas grande
	int32_t mayor_fd = principal->fd;
	FD_ZERO(&readfds_cpus);
	FD_SET(principal->fd, &readfds_cpus);

	// Preparamos el SET
	fd_set readfds = readfds_cpus;

	while(1){
		// Escuchamos el universo
		int rs = select(mayor_fd+1, &readfds, NULL, NULL, NULL);

		if(rs != -1){
			// Vemos si hay sockets para leer
			int32_t i;
			int32_t copia_mayor_fd = mayor_fd;

			for(i = 0; i < (copia_mayor_fd+1); i++){

				// Si el socket se puede leer
				if(FD_ISSET(i, &readfds)){

					if(i == principal->fd){
						// Es el socket principal, new connection knocking
					//	sock_t* nueva_conexion;

					//	if(_procesar_nueva_conexion(principal, nueva_conexion) == 1){
				//			_recalcular_mayor_fd(&mayor_fd, nueva_conexion->fd);
			//			}
					}else{
						// No es el socket principal, es un proceso
					//	_atender_socket_cpu(i);
					}
				}
			}
		}

		// Rearmamos el readfds
		readfds = readfds_cpus;
	}
	return NULL;
}

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
		case CREAME_UN_SEGMENTO:
			salida = 1;

			// programar aqui

			break;

		case DESTRUI_SEGMENTO:
			salida = 2;

			// programar aqui

			break;

		case LEE_DE_MEMORIA:
			salida = 3;

			// programar aqui

			break;

		case ESCRIBI_EN_MEMORIA:
			salida = 4;

			// programar aqui

			break;

		default:
			break;
	}

	free(msg);
	return salida;
}
