/*
 * lstestados.h
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#ifndef LSTESTADOS_H_
#define LSTESTADOS_H_

#include <commons/collections/queue.h>
#include <hu4sockets/tcb.h>

/**
 * Inicializa las colas del Planificador
 */
void inicializar_listas_estados_tcb();

/**
 * Agrega un TCB a la cola de Ready
 */
void agregar_a_ready(tcb_t* tcb);

void agregar_a_block(tcb_t* tcb);

void agregar_a_exec(tcb_t* tcb);

void agregar_a_exit(tcb_t* tcb);

void quitar_de_exec(tcb_t* tcb);

void quitar_de_block(tcb_t* tcb);

void agregar_a_cola_recurso(char* recurso, tcb_t* tcb);

tcb_t* quitar_primero_de_cola_recurso(char* recurso);

bool hay_hilo_km_ready();

tcb_t* quitar_de_ready_km();

// Quita de la queue de ready de hilos usuario.
tcb_t* quitar_de_ready();

#endif /* LSTESTADOS_H_ */
