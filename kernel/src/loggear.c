/*
 * loggear.c
 *
 *  Created on: 28/11/2014
 *      Author: martin
 */

#include "loggear.h"
#include "configuraciones.h"
#include <ansisop-panel/panel/panel.h>
#include <ansisop-panel/panel/kernel.h>
#include <hu4sockets/tcb.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include "lstestados.h"

void cargar_loggeo()
{
	inicializar_panel(KERNEL, ruta_logeo());
}

t_hilo* crear_t_hilo(tcb_t* tcb)
{
	t_hilo* hilot = malloc(sizeof(t_hilo));

	hilot->pid = tcb->pid;
	hilot->tid = tcb->tid;

	return hilot;
}

t_hilo* crear_t_hilo_con_cola(tcb_con_cola_t* tcb_cola)
{
	t_hilo* t_hilo = crear_t_hilo(tcb_cola->tcb);

	switch(tcb_cola->cola)
	{
		case MREADY:
			t_hilo->cola = READY;
			break;

		case MEXEC:
			t_hilo->cola = EXEC;
			break;

		case MBLOCK:
			t_hilo->cola = BLOCK;
			break;

		case MEXIT:
			t_hilo->cola = EXIT;
			break;

		default:
			break;
	}

	t_hilo->kernel_mode = tcb_cola->tcb->km;
	t_hilo->segmento_codigo = tcb_cola->tcb->base_codigo;
	t_hilo->segmento_codigo_size = tcb_cola->tcb->tamanio_codigo;
	t_hilo->puntero_instruccion = tcb_cola->tcb->pc;
	t_hilo->base_stack = tcb_cola->tcb->base_stack;
	t_hilo->cursor_stack = tcb_cola->tcb->cursor_stack;
	t_hilo->registros[0] = tcb_cola->tcb->a;
	t_hilo->registros[1] = tcb_cola->tcb->b;
	t_hilo->registros[2] = tcb_cola->tcb->c;
	t_hilo->registros[3] = tcb_cola->tcb->d;
	t_hilo->registros[4] = tcb_cola->tcb->e;

	return t_hilo;
}

void logear_instruccion_protegida(char* instruccion, tcb_t* tcb)
{
	t_hilo* hilot = crear_t_hilo(tcb);

	instruccion_protegida(instruccion, hilot);

	free(hilot);
}

void loggear_conexion_consola(uint32_t pid)
{
	conexion_consola(pid);
}

void loggear_desconexion_consola(uint32_t pid)
{
	desconexion_consola(pid);
}

void loggear_conexion_cpu(uint32_t id)
{
	conexion_cpu(id);
}

void loggear_desconexion_cpu(uint32_t id)
{
	desconexion_cpu(id);
}

void loggear_estado_de_hilos()
{
	// Lista de tcb_con_cola_t
	t_list* lista_tcbs = (t_list*) get_todos_los_tcbs();
	t_list* lista_t_hilos = list_create();

	void _copiar(void* elemento)
	{
		t_hilo* hilo = crear_t_hilo_con_cola(elemento);
		list_add(lista_t_hilos, hilo);
	}

	list_iterate(lista_tcbs, _copiar);

	list_clean_and_destroy_elements(lista_tcbs, free);

	hilos(lista_t_hilos);

	list_clean_and_destroy_elements(lista_t_hilos, free);

	list_destroy(lista_t_hilos);

	list_destroy(lista_tcbs);
}
