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

// Lista y mutex para conexiones sin asignar (recien recibidas)
pthread_mutex_t mutex_conexiones_unsigned = PTHREAD_MUTEX_INITIALIZER;
t_list* conexiones_unasigned;

// Lista y mutex para conexiones de procesos
pthread_mutex_t mutex_conexiones_procesos = PTHREAD_MUTEX_INITIALIZER;
t_list* conexiones_procesos;

// SET para conexiones unasigned y de procesos
fd_set readfds_unasigned_procesos;

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
	FD_SET(conexion->fd, &readfds_unasigned_procesos);

	pthread_mutex_unlock(&mutex_conexiones_unsigned);
}

void _agregar_conexion_a_procesos(sock_t* conexion)
{
	pthread_mutex_lock(&mutex_conexiones_procesos);
	//TODO: Agregar un struct para wrappear las conexiones a los procesos que tenga el PID
	list_add(conexiones_procesos, conexion);
	FD_SET(conexion->fd, &readfds_unasigned_procesos);

	pthread_mutex_unlock(&mutex_conexiones_procesos);
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

sock_t* _procesar_nueva_conexion(sock_t* principal)
{
	sock_t* nueva_conexion = aceptar_conexion(principal);

	_agregar_conexion_a_unasigned(nueva_conexion);

	return nueva_conexion;
}


void _atender_socket_unasigned_proceso(int32_t fd)
{
	//TODO: Atendeme el socket maestro
}

void _atender_socket_cpu(int32_t fd)
{
	//TODO: Atendeme el socket maestro
}

// Corre en un THREAD
void* esperar_conexiones_entrantes_y_procesos(void* un_ente)
{
	sock_t* principal = crear_socket_escuchador(puerto());
	escuchar(principal);

	// Seteamos este como el socket mas grande
	int32_t mayor_fd = principal->fd;
	FD_SET(principal->fd, &readfds_unasigned_procesos);

	// Preparamos el SET
	fd_set readfds = readfds_unasigned_procesos;

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
						sock_t* nueva_conexion = _procesar_nueva_conexion(principal);
						_recalcular_mayor_fd(&mayor_fd, nueva_conexion->fd);
					}
					else
					{// No es el socket principal, es un chiruso
						_atender_socket_unasigned_proceso(i);
					}
				}
			}
		}

		// Rearmamos el readfd
		readfds = readfds_unasigned_procesos;
	}

	return NULL;
}

// Corre en un THREAD
void* escuchar_cpus_memoria(void* otro_ente)
{

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

