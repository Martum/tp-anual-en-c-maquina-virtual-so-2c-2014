/*
 * conexion.h
 *
 *  Created on: 22/9/2014
 *      Author: martin
 */

#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include <hu4sockets/sockets.h>

typedef struct conexion_cpu {
	sock_t* socket;
	uint32_t id;
} conexion_cpu_t;

typedef struct conexion_procesos {
	sock_t* socket;
	uint32_t pid;
} conexion_procesos_t;

/**
 * Espera y despacha conexiones entrantes.
 * Se corre en un thread
 */
void* esperar_conexiones(void* p);

/**
 * Inicializa las listas para las conexiones
 */
void inicializar_listas_conexiones(void);

/**
 * Agrega una conexion a la lista de conexiones sin clasificacion
 */
void _agregar_conexion_a_unasigned(sock_t*);

/**
 * Agrega una conexion a la lista de conexiones de CPU
 */
void _agregar_conexion_a_cpu(sock_t*);

/**
 * Agrega una conexion a la lista de conexiones de Procesos
 */
void _agregar_conexion_a_procesos(sock_t*);

#endif /* CONEXIONES_H_ */
