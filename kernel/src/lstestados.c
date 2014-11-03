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
#include "planificador.h"
#include "loader.h"


/*t_queue* exec;
t_queue* block;*/

// TCB KM
tcb_t* TCB_KM;

// Cola de ready para procesos (1) y para KM (0)
t_list* READY[2];

// Cola de exit
t_list* EXIT_COLA;

// Procesos en ejecucion
t_list* EXEC;

// Procesos que estan esperando a que otro hilo termine
t_list* BLOCK_JOIN;

// Proceso que ejecuto una syscall y esta esperando a que KM la termine
t_list* BLOCK_CONCLUSION_KM;

// Procesos que estan esperando la liberacion del KM para ejecutar su propia syscall
t_list* BLOCK_ESPERA_KM;

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

	READY[0] = list_create();
	READY[1] = list_create();

	EXIT_COLA = list_create();

	EXEC = list_create();

	BLOCK_JOIN = list_create();

	BLOCK_CONCLUSION_KM = list_create();

	BLOCK_ESPERA_KM = list_create();

	DIC_COLAS_ESPERA_RECURSOS = dictionary_create();
	BLOCK_RECURSO = list_create();

	// Y esta?
	SYSCALLS_COLA = queue_create();
}


void agregar_a_ready(tcb_t* tcb) {
	list_add(READY[!tcb->km], tcb);
	// TODO: Llamar al planificador acá? Revisar en qué lugares se llama esta función.
	// Aca deberíamos llamar al planificador. No, no deberiamos. O quizas si, quien lo sabe...
}

bool hay_hilo_km_ready(){
	return !list_is_empty(READY[0]);
}

bool hay_hilo_ready(){
	return !list_is_empty(READY[1]);
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
	list_add(EXIT_COLA, tcb);
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
	return list_remove(READY[0], 0);
}

tcb_t* quitar_de_ready(){
	return list_remove(READY[1], 0);
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
	return !list_is_empty(BLOCK_ESPERA_KM);
}

void agregar_a_block_espera_km(esperando_km_t* ekm)
{
	list_add(BLOCK_ESPERA_KM, ekm);
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

/**
 * Dealloca un exit_t que busca por PID en EXIT_COLA
 */
void _eliminar_exit_t(uint32_t pid)
{
	bool _buscar_por_pid(void* elemento)
	{
		return ((exit_t*)elemento)->pid == pid;
	}

	void _destruir_exit_t(void* elemento)
	{
		exit_t* et = elemento;
		list_destroy(et->lista_tcbs);
		free(et);
	}

	list_remove_and_destroy_by_condition(EXIT_COLA, _buscar_por_pid, _destruir_exit_t);
}

/**
 * Verifica si una de las listas internas de EXIT ya esta completa (o sea,
 * ya se pueden eliminar los TCBs)
 */
void _verificar_salida_proceso(exit_t* et)
{
	if((et->muere_proceso && et->hilos_totales == list_size(et->lista_tcbs)) || !et->muere_proceso)
	{// Ya podemos eliminar los TCBs y liberar la memoria
		list_clean_and_destroy_elements(et->lista_tcbs, eliminar_y_destruir_tcb);
		_eliminar_exit_t(et->pid);
	}
}

void agregar_a_exit(tcb_t* tcb)
{
	bool _buscar_por_pid(void* elemento)
	{
		return ((exit_t*)elemento)->pid == tcb->pid;
	}

	exit_t* et = list_find(EXIT_COLA, _buscar_por_pid);

	list_add(et->lista_tcbs, tcb);

	_verificar_salida_proceso(et);
}

void remover_de_ready_a_exit(uint32_t pid)
{
	bool _satisface_pid(void* elemento)
	{
		return ((tcb_t*) elemento)->pid == pid;
	}

	uint32_t cantidad = list_count_satisfying(READY[1], _satisface_pid);

	int i;
	for(i = 0; i < cantidad; i++)
	{
		agregar_a_exit(list_remove_by_condition(READY[1], _satisface_pid));
	}
}

void preparar_exit_para_proceso(uint32_t pid, bool muere_proceso)
{
	exit_t* et = malloc(sizeof(exit_t));
	et->pid = pid;
	et->lista_tcbs = list_create();
	et->muere_proceso = muere_proceso;
	et->hilos_totales = dame_ultimo_tid(pid);

	list_add(EXIT_COLA, et);
}

void remover_de_esperando_km_a_exit(uint32_t pid)
{
	bool _satisface_pid(void* elemento)
	{
		return ((esperando_km_t*) elemento)->tcb->pid == pid;
	}

	uint32_t cantidad = list_count_satisfying(BLOCK_ESPERA_KM, _satisface_pid);

	int i;
	for(i = 0; i < cantidad; i++)
	{
		esperando_km_t* ekm = list_remove_by_condition(BLOCK_ESPERA_KM, _satisface_pid);

		agregar_a_exit(ekm->tcb);

		free(ekm);
	}
}

void remover_de_join_a_exit(uint32_t pid)
{
	bool _satisface_pid(void* elemento)
	{
		return ((esperando_join_t*) elemento)->tcb->pid == pid;
	}

	uint32_t cantidad = list_count_satisfying(BLOCK_JOIN, _satisface_pid);

	int i;
	for(i = 0; i < cantidad; i++)
	{
		esperando_join_t* ej = list_remove_by_condition(BLOCK_JOIN, _satisface_pid);

		agregar_a_exit(ej->tcb);

		free(ej);
	}
}
