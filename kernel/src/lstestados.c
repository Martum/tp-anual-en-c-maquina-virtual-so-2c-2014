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

t_queue* ready[2];
t_queue* exec;
t_queue* block;
t_queue* exit;
t_dictionary* dic_colas_espera_recursos = dictionary_create();

void inicializar_listas_estados_tcb() {
	exec = queue_create();
	block = queue_create();
	exit = queue_create();

	ready[0] = queue_create();
	ready[1] = queue_create();

	// FALTA INICIALIZAR EL DICCIONARIO DE COLAS DE ESPERA DE RECURSOS.
}

void agregar_a_ready(tcb_t* tcb) {
	queue_push(ready[!tcb->km], tcb);
	// Aca deberíamos llamar al planificador.
}

void agregar_a_block(tcb_t* tcb) {
	queue_push(block, tcb);
}

void agregar_a_exec(tcb_t* tcb) {
	queue_push(exec, tcb);
}

void agregar_a_exit(tcb_t* tcb) {
	queue_push(exit, tcb);
}

void quitar_de_exec(tcb_t* tcb) {
	// no encontré en las common un método para quitar de una queue un elemento determinado. Todo para sacar
	// el primero..
	// Aca deberíamos llamar al planificador.
}

void quitar_de_block(tcb_t* tcb) {
	// no encontré en las common un método para quitar de una queue un elemento determinado. Todo para sacar
	// el primero..
}

void agregar_a_cola_recurso(uint32_t recurso, tcb_t* tcb) {
	queue_push(dictionary_get(dic_colas_espera_recursos, recurso), tcb);
}

tcb_t* quitar_primero_de_cola_recurso(uint32_t recurso){
	/*tcb_t* tcb;
	tcb = queue_peek(dictionary_get(dic_colas_espera_recursos, recurso));
	queue_pop(dictionary_get(dic_colas_espera_recursos, recurso));

	return tcb;*/

	return queue_pop(dictionary_get(dic_colas_espera_recursos, recurso));
}
