/*
 * conexiones.c
 *
 *  Created on: 22/9/2014
 *      Author: martin
 */

#include <stdio.h>
#include <stdlib.h>

#include <hu4sockets/sockets.h>
#include <commons/collections/list.h>
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
	conexiones_cpu = list_create();
	conexiones_procesos = list_create();
}


void* esperar_conexiones(void* p)
{
	sock_t* s = crear_socket_escuchador(puerto());

	escuchar(s);

	while(1)
	{
		sock_t* fd = aceptar_conexion(s);
		procesar_conexion(fd);
	}

	return NULL;
}

void agregar_conexion_a_unasigned(sock_t*)
{

}
