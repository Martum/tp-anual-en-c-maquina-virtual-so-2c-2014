/*
 * loader.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#include <stdlib.h>

#include "loader.h"
#include "memoria.h"
#include <commons/collections/list.h>

tcb_t* crear_tcb(uint32_t pid, uint32_t tid)
{
	tcb_t* tcb = malloc(sizeof(tcb_t));

	tcb->pid = pid;
	tcb->tid = tid;

	return tcb;
}


int cargar_tcb(tcb_t* tcb, char* codigo_beso, uint32_t tamanio_codigo, uint32_t tamanio_stack)
{
	tcb->tamanio_codigo = tamanio_codigo;

	// Pedimos segmento a la MSP
	direccion dir = crear_segmento(tcb->pid, tcb->tamanio_codigo);
	direccion stack = crear_segmento(tcb->pid, tamanio_stack);

	if(dir != -1 && stack != -1)
	{// Hay memoria
		tcb->base_codigo = dir;
		tcb->pc = dir;

		tcb->base_stack = stack;
		tcb->cursor_stack = stack;

		return 0;
	}

	// No hay memoria
	return -1;
}
