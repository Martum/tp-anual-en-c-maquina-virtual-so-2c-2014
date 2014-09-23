/*
 * conexion.h
 *
 *  Created on: 22/9/2014
 *      Author: martin
 */

#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include <hu4sockets/sockets.h>
#include <sys/poll.h>

typedef struct conexion_cpu {
	sock_t* socket;
	uint32_t id;
} conexion_cpu_t;

typedef struct conexion_procesos {
	sock_t* socket;
	uint32_t pid;
} conexion_procesos_t;

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

/**
 * Procesa los fds del poll de conexiones unasigned
 */
void _procesar_poll_unasigned(struct pollfd* poll_fds, uint32_t cantidad_fds);


/**
 * Lee del socket lo que la conexion unasigned tiene para decir,
 * y la mueve a la lista que corresponde
 */
void _conexion_unasigned_lista(sock_t* conexion);

/**
 * Espera y despacha conexiones entrantes.
 * Se corre en un thread inmortal (o inmoral?).
 */
void* esperar_conexiones(void* p);

/**
 * Espera a que las conexiones sin asignacion hagan el primer contacto
 * Se corre en un thread inmortal.
 */
void* esperar_conexiones_unasigned(void* p);

/**
 * Inicializa las listas para las conexiones
 */
void inicializar_listas_conexiones(void);

#endif /* CONEXIONES_H_ */
