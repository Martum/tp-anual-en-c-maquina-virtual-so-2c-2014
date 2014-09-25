/*
 * loader.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#include <stdlib.h>
#include <stdbool.h>

#include "loader.h"
#include "memoria.h"
#include "configuraciones.h"


tcb_t* crear_tcb(uint32_t pid, uint32_t tid)
{
	tcb_t* tcb = malloc(sizeof(tcb_t));

	tcb->pid = pid;
	tcb->tid = tid;
	tcb->km = false;

	return tcb;
}

int32_t cargar_tcb_sin_codigo(tcb_t* tcb_padre, tcb_t* tcb_hijo)
{
	tcb_hijo->tamanio_codigo = tcb_padre->tamanio_codigo;

	direccion stack = crear_segmento(tcb_padre->pid, tamanio_stack());

	if(stack != -1)
	{
		tcb_hijo->base_codigo = tcb_padre->base_codigo;
		tcb_hijo->pc = tcb_padre->pc;

		tcb_hijo->base_stack = stack;
		tcb_hijo->cursor_stack = stack;

		return 0;
	}

	return -1;

}

int32_t cargar_tcb(tcb_t* tcb, char* codigo_beso, uint32_t tamanio_codigo)
{
	tcb->tamanio_codigo = tamanio_codigo;

	// Pedimos segmento a la MSP
	direccion codigo = crear_segmento(tcb->pid, tcb->tamanio_codigo);
	direccion stack = crear_segmento(tcb->pid, tamanio_stack());

	if(codigo != -1 && stack != -1)
	{// Hay memoria
		tcb->base_codigo = codigo;
		tcb->pc = codigo;

		tcb->base_stack = stack;
		tcb->cursor_stack = stack;

		return 0;
	}

	// No hay memoria
	return -1;
}
