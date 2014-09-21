/*
 * estructuras.h
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t direccion;
typedef char instruccion_t[4];

typedef struct tcb
{
	uint32_t pid;
	uint32_t tid;
	bool km;
	direccion base_codigo;
	direccion tamanio_codigo;
	direccion pc;
	direccion base_stack;
	direccion cursor_stack;
	int32_t a,b,c,d,e,f;
} tcb_t;

#endif /* ESTRUCTURAS_H_ */
