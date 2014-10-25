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
#include <stdint.h>


typedef struct ejecutando {
	tcb_t* tcb;
	uint32_t cpu;
} ejecutando_t;

/**
 * Inicializa las colas del Planificador
 */
void inicializar_listas_estados_tcb();

/**
 * Agrega un TCB a la cola de Ready
 */
void agregar_a_ready(tcb_t* tcb);

void agregar_a_block(tcb_t* tcb);

void agregar_a_exec(tcb_t* tcb, uint32_t cpu_id);

void agregar_a_exit(tcb_t* tcb);

void quitar_de_exec(tcb_t* tcb);

void quitar_de_block(tcb_t* tcb);

void agregar_a_cola_recurso(uint32_t recurso_int, tcb_t* tcb);

tcb_t* quitar_primero_de_cola_recurso(uint32_t recurso_int);

bool hay_hilo_km_ready();

tcb_t* quitar_de_ready_km();

// Quita de la queue de ready de hilos usuario.
tcb_t* quitar_de_ready();

/**
 * @RETURNS: Devuelve el identificador de recurso como char*
 */
char* identificador_de_recurso(uint32_t identificador);

/**
 * Busca en la lista de Exec el ejecutando_t de un determinado Hilo
 */
ejecutando_t* buscar_exec_por_pid_tid(uint32_t pid, uint32_t tid);

#endif /* LSTESTADOS_H_ */
