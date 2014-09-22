/*
 * lstestados.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#include "lstestados.h"
#include <stdbool.h>
#include <commons/collections/queue.h>

t_queue* ready[2];
t_queue* exec;
t_queue* block;
t_queue* exit;

void inicializar_listas_estados_tcb()
{
	exec = queue_create();
	block = queue_create();
	exit = queue_create();

	ready[0] = queue_create();
	ready[1] = queue_create();
}

void agregar_a_ready(tcb_t* tcb)
{
	queue_push(ready[!tcb->km], tcb);
}

void agregar_a_block(tcb_t* tcb)
{
	queue_push(block, tcb);
}

void agregar_a_exec(tcb_t* tcb)
{
	queue_push(exec, tcb);
}

void agregar_a_exit(tcb_t* tcb)
{
	queue_push(exit, tcb);
}
