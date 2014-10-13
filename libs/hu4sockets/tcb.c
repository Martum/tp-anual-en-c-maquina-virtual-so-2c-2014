/*
 * tcb.c
 *
 *  Created on: 13/10/2014
 *      Author: martin
 */


#include "tcb.h"

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

uint32_t tamanio_tcb_serializado()
{
	uint32_t t = 0;
	t += sizeof(uint32_t) * 2;
	t += sizeof(bool);
	t += sizeof(direccion) * 5;
	t += sizeof(int32_t) * 6;

	return t;
}

char* serializar_tcb(tcb_t* tcb)
{
	char* bytes = malloc(tamanio_tcb_serializado());

	uint32_t offset = 0;
	memcpy(bytes+offset,&tcb->pid, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &tcb->tid, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &tcb->km, sizeof(bool));

	offset += sizeof(bool);
	memcpy(bytes + offset, &tcb->base_codigo, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(bytes + offset, &tcb->tamanio_codigo, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(bytes + offset, &tcb->pc, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(bytes + offset, &tcb->base_stack, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(bytes + offset, &tcb->cursor_stack, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(bytes + offset, &tcb->a, sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(bytes + offset, &tcb->b, sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(bytes + offset, &tcb->c, sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(bytes + offset, &tcb->d, sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(bytes + offset, &tcb->e, sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(bytes + offset, &tcb->f, sizeof(int32_t));


}

