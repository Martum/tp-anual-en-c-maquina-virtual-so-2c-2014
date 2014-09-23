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
pthread_mutex_t mutex_conexiones_unsigned;
t_list* conexiones_unasigned;

// Lista y mutex para conexiones cpu
pthread_mutex_t mutex_conexiones_cpu;
t_list* conexiones_cpu;

// Lista y mutex para conexiones de procesos
pthread_mutex_t mutex_conexiones_procesos;
t_list* conexiones_procesos;

// Socket y mutex de la conexion con memoria
pthread_mutex_t mutex_conexion_memoria;
sock_t* conexion_memoria;

void _agregar_conexion_a_unasigned(sock_t* conexion)
{
	pthread_mutex_lock(&mutex_conexiones_unsigned);

	list_add(conexiones_unasigned, conexion);

	pthread_mutex_unlock(&mutex_conexiones_unsigned);
}

void _agregar_conexion_a_cpu(sock_t* conexion)
{
	pthread_mutex_lock(&mutex_conexiones_cpu);

	list_add(conexiones_cpu, conexion);

	pthread_mutex_unlock(&mutex_conexiones_cpu);
}

void _agregar_conexion_a_procesos(sock_t* conexion)
{
	pthread_mutex_lock(&mutex_conexiones_procesos);
	//TODO: Agregar un struct para wrappear las conexiones a los procesos que tenga el PID
	list_add(conexiones_procesos, conexion);

	pthread_mutex_unlock(&mutex_conexiones_procesos);
}

void _procesar_poll_unasigned(struct pollfd* poll_fds, uint32_t cantidad_fds)
{
	int i;
	for(i = 0; i < cantidad_fds; i++)
	{
		// Funcion para encontrar el elemento deseado
		int fd = poll_fds->fd;
		bool encontrar_socket(void* elemento)
		{
			sock_t* socket = (sock_t*) elemento;
			return socket->fd == fd;
		}

		if(poll_fds->revents & POLLIN)
		{// Podemos leer

			// Encontramos el elemento
			sock_t* socket = list_find(conexiones_unasigned, encontrar_socket);

			// Procesamos una conexion unasigned que esta lista para hablarnos
			_conexion_unasigned_lista(socket);
		}
		else if((poll_fds->revents & POLLERR) || (poll_fds->revents & POLLHUP))
		{// Error o socket cerrado, lo volamos a la mierda
			list_remove_and_destroy_by_condition(conexiones_unasigned, encontrar_socket, cerrar_liberar_generico);
		}

		poll_fds++;
	}
}

void inicializar_listas_conexiones(void)
{
	conexiones_unasigned = list_create();
	pthread_mutex_init(&mutex_conexiones_unsigned, NULL);

	conexiones_cpu = list_create();
	pthread_mutex_init(&mutex_conexiones_cpu, NULL);

	conexiones_procesos = list_create();
	pthread_mutex_init(&mutex_conexiones_procesos, NULL);

	pthread_mutex_init(&mutex_conexion_memoria, NULL);
}

// Corre en un thread principal
void* esperar_conexiones(void* p)
{
	sock_t* s = crear_socket_escuchador(puerto());

	escuchar(s);

	while(1)
	{
		sock_t* nueva_conexion = aceptar_conexion(s);
		_agregar_conexion_a_unasigned(nueva_conexion);
	}

	return NULL;
}

// Corre en un thread
void* esperar_conexiones_unasigned(void* p)
{
	while(1)
	{
		pthread_mutex_lock(&mutex_conexiones_unsigned);

		uint32_t tamanio_lista = list_size(conexiones_unasigned);

		// Creamos el poll de conexiones
		struct pollfd* poll_fds = malloc(sizeof(struct pollfd) * tamanio_lista);
		struct pollfd* poll_fds_copia = poll_fds;

		void agregar_a_fds(void* elemento)
		{
			// Seteamos el fd para que nos notifique sobre lectura
			sock_t* socket = (sock_t*) elemento;
			poll_fds_copia->fd = socket->fd;
			poll_fds_copia->events = POLLIN;

			// Adelantamos le puntero al siguiente struct
			poll_fds_copia++;
		}

		list_iterate(conexiones_unasigned, agregar_a_fds);

		pthread_mutex_unlock(&mutex_conexiones_unsigned);

		// Hacemos poll. Si hay movimientos, los procesamos
		int rv = poll(poll_fds, tamanio_lista, 3500);
		if(rv != -1 && rv != 0)
			_procesar_poll_unasigned(poll_fds, tamanio_lista);

		// Limpiamos la lista del poll
		free(poll_fds);
	}

	return NULL;
}
