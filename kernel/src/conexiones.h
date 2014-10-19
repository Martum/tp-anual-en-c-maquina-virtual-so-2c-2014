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
} conexion_proceso_t;

/**
 * Agrega una conexion a la lista de conexiones de CPU
 */
void _agregar_conexion_a_cpu(sock_t*, uint32_t id);

/**
 * Agrega una conexion a la lista de conexiones de Procesos
 */
void _agregar_conexion_a_procesos(sock_t*, uint32_t pid);


/**
 * Lee del socket lo que la conexion unasigned tiene para decir,
 * y la mueve a la lista que corresponde
 */
void _conexion_unasigned_lista(sock_t* conexion);

/**
 * Escucha por conexiones entrantes nuevas y por contactos desde procesos
 * Se corre en un thread inmortal (o inmoral?).
 */
void* escuchar_conexiones_entrantes_y_procesos(void* p);

/**
 * Escucha las solicitudes de los CPUs
 * Se corre en un thread inmortal.
 */
void* escuchar_cpus(void* p);

/**
 * Inicializa las listas para las conexiones
 */
void inicializar_listas_conexiones(void);

/**
 * Busca el sock_t* correspondiende a un proceso por su PID
 *
 * @RETURNS: El sock_t* del proceso identificado por el PID
 */
sock_t* buscar_conexion_proceso_por_pid(uint32_t pid);

/**
 * Busca el sock_t* correspondiente a un proceso por su FD
 *
 * @RETURNS: El sock_t* del proceso correspondiente al FD
 */
sock_t* buscar_conexion_proceso_por_fd(int32_t fd);

/**
 * Busca el sock_t* correspondiente a una conexion por su FD
 *
 * @RETURNS: El sock_t* de la conexion unasigned correspondiente al FD
 */
sock_t* buscar_conexion_unasigned_por_fd(int32_t fd);

#endif /* CONEXIONES_H_ */
