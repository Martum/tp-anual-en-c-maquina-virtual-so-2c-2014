/*
 * mensajes.h
 *
 *  Created on: 24/09/2014
 *      Author: utnso
 */

#include <stdbool.h>
#include <stdint.h>

#ifndef MENSAJES_H_
#define MENSAJES_H_

typedef uint32_t direccion;

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

typedef enum {MANDA_TCB, TOMA_TCB} flag_t;

typedef struct mensaje_request {
	flag_t flag;
} mensaje_request_t;

typedef struct mensaje_response_tcb {
	flag_t flag;
	tcb_t tcb;
	uint32_t quantum;
} mensaje_response_tcb_t;

#endif /* MENSAJES_H_ */
