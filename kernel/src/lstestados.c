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


/*t_queue* exec;
t_queue* block;*/

t_queue* ready[2];
t_queue* exit;
t_list* exec;
t_list* block;
t_dictionary* dic_colas_espera_recursos;
t_queue* syscalls;

void inicializar_listas_estados_tcb() {

	exit = queue_create();
	syscalls = queue_create();

	ready[0] = queue_create();
	ready[1] = queue_create();

	exec = list_create();
	block = list_create();

	dic_colas_espera_recursos = dictionary_create();

	//TODO: FALTA INICIALIZAR EL DICCIONARIO DE COLAS DE ESPERA DE RECURSOS.
}


void agregar_a_ready(tcb_t* tcb) {
	queue_push(ready[!tcb->km], tcb);
	// Aca deberíamos llamar al planificador.
}

bool hay_hilo_km_ready(){
	return !queue_is_empty(ready[0]);
}

void agregar_a_block(tcb_t* tcb) {
	list_add(block, tcb);
}

void agregar_a_syscalls(tcb_t* tcb) {
	queue_push(syscalls, tcb);
}

void agregar_a_exec(tcb_t* tcb) {
	list_add(exec, tcb);
}

void agregar_a_exit(tcb_t* tcb) {
	queue_push(exit, tcb);
}

void quitar_de_exec(tcb_t* tcb) {

	bool _igual_tid(void* elemento){
		return tcb->tid == ((tcb_t*) elemento)->tid;
	}

	list_remove_by_condition(exec, _igual_tid );
}

void quitar_de_block(tcb_t* tcb) {

	bool _igual_tid(void* elemento){
		return tcb->tid == ((tcb_t*) elemento)->tid;
	}

	list_remove_by_condition(block, _igual_tid);
}

void agregar_a_cola_recurso(uint32_t recurso, tcb_t* tcb) {
	queue_push(dictionary_get(dic_colas_espera_recursos, recurso), tcb);
}

void quitar_de_syscalls(tcb_t* tcb) {
	queue_pop(syscalls);
}

tcb_t* quitar_primero_de_cola_recurso(uint32_t recurso){
	return queue_pop(dictionary_get(dic_colas_espera_recursos, recurso));
}

tcb_t* quitar_de_ready_km(){
	return queue_pop(ready[0]);
}

tcb_t* quitar_de_ready(){
	return queue_pop(ready[1]);
}
