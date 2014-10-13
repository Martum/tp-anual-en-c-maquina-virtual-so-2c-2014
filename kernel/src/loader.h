/*
 * loader.h
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#ifndef LOADER_H_
#define LOADER_H_

#include <stdint.h>
#include <stdbool.h>
#include <hu4sockets/tcb.h>

#include "memoria.h"


/**
 * @RETURNS: Un nuevo PID que no esta en uso
 */
uint32_t dame_nuevo_pid();

/**
 * @RETURNS: Un nuevo TID que no esta en uso
 */
uint32_t dame_nuevo_tid();

/**
 * Procesa un nuevo programa que acaba de conectarse con nosotros.
 * Creamos el TCB, le asignamos PID y TID (como es el primero TID=1).
 * Pedimos memoria y cargamos el BESO ;)
 *
 * @RETURNS: Devuelve el PID del programa; o -1 si no hay memoria (segui participando, campeon)
 */
int32_t procesar_nuevo_programa(char* codigo_beso, uint32_t len);

/**
 * @DESC: Crea un TCB asignandole con el PID recibido y TID
 */
tcb_t* crear_tcb(uint32_t pid, uint32_t tid);


// DEPRECATED
/**
 * Copia un TCB asignandole un nuevo Segmento de Stack
 */
//int32_t cargar_tcb_sin_codigo(tcb_t* tcb_padre, tcb_t* tcb_hijo);

#endif /* LOADER_H_ */
