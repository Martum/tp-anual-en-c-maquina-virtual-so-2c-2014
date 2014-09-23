/*
 * conexiones.c
 *
 *  Created on: 22/9/2014
 *      Author: martin
 */

#include <stdio.h>
#include <stdlib.h>

#include <commons/collections/list.h>
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
		agregar_conexion_a_unasigned(nueva_conexion);
	}

	return NULL;
}

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
