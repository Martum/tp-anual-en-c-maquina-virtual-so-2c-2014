/*
 * lstestados.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#include "lstestados.h"
#include <stdbool.h>
#include <commons/collections/queue.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <stdlib.h>


/*t_queue* exec;
t_queue* block;*/

t_queue* READY[2];
t_queue* EXIT_COLA;
t_list* EXEC;
t_list* BLOCK;
t_queue* BLOCK_ESPERA_KM; // hacer getter y setters.
t_dictionary* DIC_COLAS_ESPERA_RECURSOS;
t_queue* SYSCALLS_COLA;

void _eliminar_tcb(void* elemento)
{
	free((tcb_t*) elemento);
}

void inicializar_listas_estados_tcb() {

	EXIT_COLA = queue_create();
	SYSCALLS_COLA = queue_create();

	READY[0] = queue_create();
	READY[1] = queue_create();

	EXEC = list_create();
	BLOCK = list_create();

	DIC_COLAS_ESPERA_RECURSOS = dictionary_create();

	//TODO: FALTA INICIALIZAR EL DICCIONARIO DE COLAS DE ESPERA DE RECURSOS.
}


void agregar_a_ready(tcb_t* tcb) {
	queue_push(READY[!tcb->km], tcb);
	// Aca deberíamos llamar al planificador.
}

bool hay_hilo_km_ready(){
	return !queue_is_empty(READY[0]);
}

void agregar_a_block(tcb_t* tcb) {
	list_add(BLOCK, tcb);
}

void agregar_a_syscalls_cola(tcb_t* tcb) {
	queue_push(SYSCALLS_COLA, tcb);
}

void agregar_a_exec(tcb_t* tcb, uint32_t cpu_id) {
	ejecutando_t* ej = malloc(sizeof(ejecutando_t));
	ej->tcb = tcb;
	ej->cpu = cpu_id;

	list_add(EXEC, ej);
}

void agregar_a_exit_cola(tcb_t* tcb) {
	queue_push(EXIT_COLA, tcb);
}

tcb_t* quitar_de_exec(tcb_t* tcb) {

	bool _igual_tid(void* elemento){
		return tcb->tid == ((ejecutando_t*) elemento)->tcb->tid;
	}

	return (tcb_t*) list_remove_by_condition(EXEC, _igual_tid );
	//list_remove_and_destroy_by_condition(EXEC, _igual_tid, _eliminar_tcb);
}

void quitar_de_block(tcb_t* tcb) {

	bool _igual_tid(void* elemento){
		return tcb->tid == ((tcb_t*) elemento)->tid;
	}

	list_remove_by_condition(BLOCK, _igual_tid);
}

void agregar_a_cola_recurso(char* recurso, tcb_t* tcb) {
	queue_push((t_queue*)dictionary_get(DIC_COLAS_ESPERA_RECURSOS, recurso), tcb);
}

void quitar_de_syscalls_cola(tcb_t* tcb) {
	queue_pop(SYSCALLS_COLA);
}

tcb_t* quitar_primero_de_cola_recurso(char* recurso){
	return queue_pop((t_queue*)dictionary_get(DIC_COLAS_ESPERA_RECURSOS, recurso));
}

tcb_t* quitar_de_ready_km(){
	return queue_pop(READY[0]);
}

tcb_t* quitar_de_ready(){
	return queue_pop(READY[1]);
}
