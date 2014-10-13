/*
 * tcb.c
 *
 *  Created on: 13/10/2014
 *      Author: martin
 */


#include "tcb.h"
#include <stdlib.h>
#include <string.h>

tcb_t* deserializar_tcb(char* chorro)
{
	tcb_t* tcb = malloc(sizeof(tcb_t));

	uint32_t offset = 0;
	memcpy(&tcb->pid, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&tcb->tid, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&tcb->km, chorro + offset, sizeof(bool));

	offset += sizeof(bool);
	memcpy(&tcb->base_codigo, chorro + offset, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(&tcb->tamanio_codigo, chorro + offset, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(&tcb->pc, chorro + offset, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(&tcb->base_stack, chorro + offset, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(&tcb->cursor_stack, chorro + offset, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(&tcb->a, chorro + offset, sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(&tcb->b, chorro + offset, sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(&tcb->c, chorro + offset, sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(&tcb->d, chorro + offset, sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(&tcb->e, chorro + offset, sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(&tcb->f, chorro + offset, sizeof(int32_t));

	return tcb;
}

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

	return bytes;
}

