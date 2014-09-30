/*
 * tcb.h
 *
 *  Created on: 27/09/2014
 *      Author: utnso
 */

#include <stdbool.h>
#include <stdint.h>

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

#endif /* TCB_H_ */
