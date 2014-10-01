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

uint32_t pid_global = 1;
uint32_t tid_global = 1;

uint32_t dame_nuevo_pid()
{
	return pid_global++;
}

uint32_t dame_nuevo_tid()
{
	return tid_global++;
}


int32_t procesar_nuevo_programa(char* codigo_beso, uint32_t len)
{
	tcb_t* tcb = crear_tcb(dame_nuevo_pid(), dame_nuevo_tid());
	tcb->tamanio_codigo = len;

	// Pido segmento de codigo
	direccion direccion_segmento_codigo;
	int32_t resultado = crear_segmento(tcb->pid, len, &direccion_segmento_codigo);

	// Si no hay memoria devolvemos
	if(resultado == -1)
	{
		free(tcb);
		return -1;
	}

	// Pido segmento stack
	direccion direccion_segmento_stack;
	resultado = crear_segmento(tcb->pid, tamanio_stack(), &direccion_segmento_stack);

	// Si no hay memoria devolvemos
	if(resultado == -1)
	{
		destruir_segmento(tcb->pid, direccion_segmento_codigo);
		free(tcb);
		return -1;
	}


	// Seteamos variables del TCB
	tcb->base_codigo = direccion_segmento_codigo;
	tcb->pc = direccion_segmento_codigo;

	tcb->base_stack = direccion_segmento_stack;
	tcb->cursor_stack = direccion_segmento_stack;

	// Escribimos en la memoria el codigo BESO
	// No puede haber SEGMENTATION FAULT porque pedimos el Segmento con este LEN
	escribir_memoria(tcb->pid, tcb->base_codigo, codigo_beso, len);

	// Agregamos el TCB a rdy
	agregar_a_ready(tcb);

	return 0;
}


tcb_t* crear_tcb(uint32_t pid, uint32_t tid)
{
	tcb_t* tcb = malloc(sizeof(tcb_t));

	tcb->pid = pid;
	tcb->tid = tid;
	tcb->km = false;

	return tcb;
}

// DEPRECATED
/*int32_t cargar_tcb_sin_codigo(tcb_t* tcb_padre, tcb_t* tcb_hijo)
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

}*/
