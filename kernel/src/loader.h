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
 * @NAME: crear_tcb
 * @DESC: Crea un TCB asignandole con el PID recibido y TID
 */
tcb_t* crear_tcb(uint32_t pid, uint32_t tid);

/**
 * @NAME: cargar_tcb
 * @DESC: Carga un TCB con la base del segmento de codigo y stack.
 * 		Devuelve 0 en caso de exito y -1 en caso de no poder alocar la memoria necesaria.
 */
int cargar_tcb(tcb_t* tcb, char* codigo_beso, uint32_t tamanio_codigo);

/**
 * Copia un TCB asignandole un nuevo Segmento de Stack
 */
int cargar_tcb_sin_codigo(tcb_t* tcb_padre, tcb_t* tcb_hijo);

#endif /* LOADER_H_ */
