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
#include "estructuras.h"


/**
 * Procesa un nuevo programa que acaba de conectarse con nosotros
 *
 * @RETURNS: Devuelve el PID del programa; o -1 si no hay memoria (segui participando, campeon)
 */
int32_t procesar_nuevo_programa(char* codigo_beso);

/**
 * @DESC: Crea un TCB asignandole con el PID recibido y TID
 */
tcb_t* crear_tcb(uint32_t pid, uint32_t tid);

/**
 * @DESC: Carga un TCB con la base del segmento de codigo y stack.
 *
 * @RETURNS: 0 en caso de exito; o -1 en caso de no poder alocar la memoria necesaria.
 */
int32_t cargar_tcb(tcb_t* tcb, char* codigo_beso, uint32_t tamanio_codigo);

/**
 * Copia un TCB asignandole un nuevo Segmento de Stack
 */
int32_t cargar_tcb_sin_codigo(tcb_t* tcb_padre, tcb_t* tcb_hijo);

#endif /* LOADER_H_ */
