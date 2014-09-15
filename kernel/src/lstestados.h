/*
 * lstestados.h
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#ifndef LSTESTADOS_H_
#define LSTESTADOS_H_

#include <commons/collections/queue.h>
#include "estructuras.h"

/**
 * Inicializa las colas del Planificador
 */
void inicializar_listas();

/**
 * Agrega un TCB a la cola de Ready
 */
void agregar_a_ready(tcb_t* tcb);

void agregar_a_block(tcb_t* tcb);

void agregar_a_exec(tcb_t* tcb);

void agregar_a_exit(tcb_t* tcb);


#endif /* LSTESTADOS_H_ */
