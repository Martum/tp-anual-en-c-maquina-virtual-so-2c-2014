/*
 * conexion.h
 *
 *  Created on: 22/9/2014
 *      Author: martin
 */

#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include <hu4sockets/sockets.h>

// Struct para conexiones CPU
typedef struct conexion_cpu {
	sock_t* socket;
	uint32_t id;
} conexion_cpu_t;

// Struct para conexiones Procesos
typedef struct conexion_procesos {
	sock_t* socket;
	uint32_t pid;
} conexion_proceso_t;

/**
 * Escucha por conexiones entrantes nuevas y por contactos desde procesos
 * Se corre en un thread inmortal (o inmoral?).
 */
void* escuchar_conexiones_entrantes_y_procesos(void* p);

/**
 * Elimina la conexion de la consola de la lista
 */
void eliminar_conexion_proceso(sock_t*);

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
 * Bloquea el MUTEX para Exit
 */
void bloquear_exit();

/**
 * Desbloquea el MUTEX para Exit
 */
void desbloquear_exit();


// ********* BUSQUEDA DE PROCESOS ***************

/**
 * Busca el sock_t* correspondiende a un proceso por su PID
 *
 * @RETURNS: El sock_t* del proceso identificado por el PID
 */
sock_t* buscar_conexion_proceso_por_pid(uint32_t pid);

/**
 * Busca el conexion_proceso_t* correspondiente a un proceso por su FD
 *
 * @RETURNS: El conexion_proceso_t* del proceso
 */
conexion_proceso_t* buscar_proceso_por_fd(int32_t fd);

/**
 * Busca el sock_t* correspondiente a un proceso por su FD
 *
 * @RETURNS: El sock_t* del proceso correspondiente al FD
 */
sock_t* buscar_conexion_proceso_por_fd(int32_t fd);


// ********* BUSQUEDA DE CPUs ***************

/**
 * Busca el sock_t* del CPU correspondiente al ID
 */
sock_t* buscar_conexion_cpu_por_id(uint32_t id);

/**
 * Busca el conexion_cpu_t* del CPU correspondiente al FD
 */
conexion_cpu_t* buscar_cpu_por_fd(int32_t fd);

/**
 * Busca el sock_t* del CPU correspondiente al FD
 */
sock_t* buscar_conexion_cpu_por_fd(int32_t fd);

#endif /* CONEXIONES_H_ */
