/*
 * loader.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lstestados.h"
#include "loader.h"
#include <commons/collections/list.h>
#include "configuraciones.h"
#include <pthread.h>

#include "memoria.h"

#include "loggear.h"

uint32_t PID_GLOBAL = 1;
pthread_mutex_t MUTEX_PID = PTHREAD_MUTEX_INITIALIZER;

t_list* PIDS = NULL;


uint32_t dame_nuevo_pid()
{
	pthread_mutex_lock(&MUTEX_PID);

	if(PIDS == NULL)
		PIDS = list_create();

	pid_lista_t* pidt = malloc(sizeof(pid_lista_t));
	pidt->pid = ++PID_GLOBAL;
	pidt->ultimo_tid = 0;

	list_add(PIDS, pidt);

	pthread_mutex_unlock(&MUTEX_PID);

	return pidt->pid;
}

uint32_t dame_nuevo_tid(uint32_t pid)
{
	bool _buscar_por_pid(void* elemento)
	{
		return ((pid_lista_t*) elemento)->pid == pid;
	}

	pthread_mutex_lock(&MUTEX_PID);

	pid_lista_t* pidt = list_find(PIDS, _buscar_por_pid);
	pidt->ultimo_tid = pidt->ultimo_tid + 1;

	pthread_mutex_unlock(&MUTEX_PID);

	return pidt->ultimo_tid;
}

uint32_t dame_ultimo_tid(uint32_t pid)
{
	bool _buscar_por_pid(void* elemento)
	{
		return ((pid_lista_t*) elemento)->pid == pid;
	}

	pthread_mutex_lock(&MUTEX_PID);

	pid_lista_t* pidt = list_find(PIDS, _buscar_por_pid);

	pthread_mutex_unlock(&MUTEX_PID);

	return pidt->ultimo_tid;
}

int32_t procesar_nuevo_programa(char* codigo_beso, uint32_t len)
{
	uint32_t pid = dame_nuevo_pid();
	tcb_t* tcb = crear_tcb(pid, dame_nuevo_tid(pid));
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
	inicializar_tcb(tcb);

	// Escribimos en la memoria el codigo BESO
	// No puede haber SEGMENTATION FAULT porque pedimos el Segmento con este LEN
	if(escribir_memoria(tcb->pid, tcb->base_codigo, codigo_beso, len) == -1)
	{
		return -1;
	}

	loggear_conexion_consola(pid);

	// Agregamos el TCB a rdy
	agregar_a_ready(tcb);

	return pid;
}


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

	direccion stack;
	int32_t res = crear_segmento(tcb_padre->pid, tamanio_stack(), &stack);

	if(res != -1)
	{
		tcb_hijo->base_codigo = tcb_padre->base_codigo;
		tcb_hijo->pc = tcb_padre->pc;

		tcb_hijo->base_stack = stack;
		tcb_hijo->cursor_stack = stack;

		return 0;
	}

	return -1;

}
