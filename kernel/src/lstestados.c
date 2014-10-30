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
#include <stdio.h>


/*t_queue* exec;
t_queue* block;*/

// TCB KM
tcb_t* TCB_KM;

// Cola de ready para procesos (1) y para KM (0)
t_queue* READY[2];

// Cola de exit
t_queue* EXIT_COLA;

// Procesos en ejecucion
t_list* EXEC;

// Procesos que estan esperando a que otro hilo termine
t_list* BLOCK_JOIN;

// Proceso que ejecuto una syscall y esta esperando a que KM la termine
t_list* BLOCK_CONCLUSION_KM;

// Procesos que estan esperando la liberacion del KM para ejecutar su propia syscall
t_queue* BLOCK_ESPERA_KM;

// Diccionario con identificadores de recursos y colas que estan esperando liberacion
t_dictionary* DIC_COLAS_ESPERA_RECURSOS;
t_list* BLOCK_RECURSO;


t_queue* SYSCALLS_COLA; // Que es esto?

void _eliminar_tcb(void* elemento)
{
	free((tcb_t*) elemento);
}

void inicializar_listas_estados_tcb()
{
	TCB_KM = malloc(sizeof(tcb_t));
	TCB_KM->km = true;

	READY[0] = queue_create();
	READY[1] = queue_create();

	EXIT_COLA = queue_create();

	EXEC = list_create();

	BLOCK_JOIN = list_create();

	BLOCK_CONCLUSION_KM = list_create();

	BLOCK_ESPERA_KM = queue_create();

	DIC_COLAS_ESPERA_RECURSOS = dictionary_create();
	BLOCK_RECURSO = list_create();

	// Y esta?
	SYSCALLS_COLA = queue_create();
}


void agregar_a_ready(tcb_t* tcb) {
	queue_push(READY[!tcb->km], tcb);
	// TODO: Llamar al planificador acá? Revisar en qué lugares se llama esta función.
	// Aca deberíamos llamar al planificador. No, no deberiamos. O quizas si, quien lo sabe...
}

bool hay_hilo_km_ready(){
	return !queue_is_empty(READY[0]);
}

bool hay_hilo_ready(){
	return !queue_is_empty(READY[1]);
}

void agregar_a_block_recurso(tcb_t* tcb)
{
	list_add(BLOCK_RECURSO, tcb);
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

void destruir_ejecutando(void* elemento)
{
	ejecutando_t* ej = (ejecutando_t*)elemento;
	free(ej->tcb);
	free(ej);
}

tcb_t* quitar_de_exec(tcb_t* tcb) {

	bool _igual_pid_tid(void* elemento){
		return tcb->pid == ((ejecutando_t*) elemento)->tcb->pid
				&& tcb->tid == ((ejecutando_t*) elemento)->tcb->tid;
	}

	// Buscamos el ejecutando_t, nos guardamos la referencia al tcb_t y liberamos
	ejecutando_t* ejecutando = list_remove_by_condition(EXEC, _igual_pid_tid );
	tcb_t* tcb_salida = ejecutando->tcb;
	free(ejecutando);

	return tcb_salida;

	//list_remove_and_destroy_by_condition(EXEC, _igual_pid_tid, destruir_ejecutando);
}


void quitar_de_block_recurso(tcb_t* tcb)
{

	bool _igual_pid_tid(void* elemento){
		return tcb->pid == ((tcb_t*) elemento)->pid &&
			tcb->tid == ((tcb_t*) elemento)->tid;
	}

	list_remove_by_condition(BLOCK_RECURSO, _igual_pid_tid);
}

void agregar_a_cola_recurso(uint32_t recurso_int, tcb_t* tcb)
{
	char* recurso = identificador_de_recurso(recurso_int);

	if(!dictionary_has_key(DIC_COLAS_ESPERA_RECURSOS, recurso))
		dictionary_put(DIC_COLAS_ESPERA_RECURSOS, recurso, queue_create());

	queue_push((t_queue*)dictionary_get(DIC_COLAS_ESPERA_RECURSOS, recurso), tcb);

	free(recurso);
}

// TODO: Recibe TCB y no hace nada con eso??
void quitar_de_syscalls_cola(tcb_t* tcb) {
	queue_pop(SYSCALLS_COLA);
}

tcb_t* quitar_primero_de_cola_recurso(uint32_t recurso_int)
{
	char* recurso = identificador_de_recurso(recurso_int);

	tcb_t* tcb = queue_pop((t_queue*)dictionary_get(DIC_COLAS_ESPERA_RECURSOS, recurso));

	free(recurso);
	return tcb;
}

tcb_t* quitar_de_ready_km(){
	return queue_pop(READY[0]);
}

tcb_t* quitar_de_ready(){
	return queue_pop(READY[1]);
}

char* identificador_de_recurso(uint32_t identificador_int)
{
	char* identificador = malloc(12);
	sprintf(identificador, "%d", identificador_int);

	return identificador;
}

ejecutando_t* buscar_exec_por_pid_tid(uint32_t pid, uint32_t tid)
{
	bool _buscar_pid_tid(void* elemento)
	{
		return ((ejecutando_t*) elemento)->tcb->pid == pid && ((ejecutando_t*) elemento)->tcb->tid == tid;
	}

	return list_find(EXEC, _buscar_pid_tid);
}

tcb_t* get_tcb_km()
{
	return TCB_KM;
}

bool tcb_km_is_running()
{
	return !list_is_empty(BLOCK_CONCLUSION_KM);
}

bool hay_hilos_block_espera_km()
{
	return !queue_is_empty(BLOCK_ESPERA_KM);
}

void agregar_a_block_espera_km(esperando_km_t* ekm)
{
	queue_push(BLOCK_ESPERA_KM, ekm);
}

void agregar_a_block_conclusion_km(tcb_t* tcb)
{
	conclusion_km_t* ckm = malloc(sizeof(conclusion_km_t));
	ckm->tcb = tcb;
	ckm->enviar_a_rdy = true;

	list_add(BLOCK_CONCLUSION_KM, ckm);
}

void set_enviar_a_rdy(bool un_bool)
{
	conclusion_km_t* ckm = (conclusion_km_t*)list_get(BLOCK_CONCLUSION_KM, 0);
	ckm->enviar_a_rdy = un_bool;
}

tcb_t* get_bloqueado_conclusion_tcb()
{
	return ((conclusion_km_t*)list_get(BLOCK_CONCLUSION_KM, 0))->tcb;
}

void agregar_a_block_join(esperando_join_t* ej)
{
	list_add(BLOCK_JOIN, ej);
}
