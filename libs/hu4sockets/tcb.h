/*
 * tcb.h
 *
 *  Created on: 27/09/2014
 *      Author: utnso
 */

#include <stdbool.h>
#include <stdint.h>
#include "resultados.h"

#ifndef TCB_H_
#define TCB_H_

typedef uint32_t direccion;

typedef struct tcb {
	uint32_t pid;
	uint32_t tid;
	bool km;
	direccion base_codigo;
	direccion tamanio_codigo;
	direccion pc;
	direccion base_stack;
	direccion cursor_stack;
	int32_t a, b, c, d, e, f;
} tcb_t;

/**
 * Serializa un TCB a Chorro de Bytes
 * @RETURNS: El chorro de bytes para enviar por la red
 */
char* serializar_tcb(tcb_t* tcb);

uint32_t tamanio_tcb_serializado();

/**
 * Deserializa un chorro de bytes a TCB
 * @RETURNS: El TCB
 */
tcb_t* deserializar_tcb(char* tcb_chorreado);

#endif /* TCB_H_ */
