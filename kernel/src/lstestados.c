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
#include "memoria.h"
#include "cpu.h"
#include <pthread.h>


/*t_queue* exec;
t_queue* block;*/

// TCB KM
tcb_t* TCB_KM;

// Cola de ready para procesos (1) y para KM (0)
t_list* READY_COLA[2];
pthread_mutex_t MUTEX_READY = PTHREAD_MUTEX_INITIALIZER;

// Cola de exit
t_list* EXIT_COLA;

// Procesos en ejecucion
t_list* EXEC_COLA;
pthread_mutex_t MUTEX_EXEC = PTHREAD_MUTEX_INITIALIZER;

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

void bloquear_ready()
{
	pthread_mutex_lock(&MUTEX_READY);
}

void desbloquear_ready()
{
	pthread_mutex_unlock(&MUTEX_READY);
}

void bloquear_exec()
{
	pthread_mutex_lock(&MUTEX_EXEC);
}

void desbloquear_exec()
{
	pthread_mutex_unlock(&MUTEX_EXEC);
}

void _eliminar_tcb(void* elemento)
{
	free((tcb_t*) elemento);
}

void inicializar_listas_estados_tcb()
{
	TCB_KM = malloc(sizeof(tcb_t));
	TCB_KM->km = true;
	TCB_KM->base_codigo = 0;
	inicializar_tcb(TCB_KM);

	READY_COLA[0] = list_create();
	READY_COLA[1] = list_create();

	EXIT_COLA = list_create();

	EXEC_COLA = list_create();

	BLOCK_JOIN = list_create();

	BLOCK_CONCLUSION_KM = list_create();

	BLOCK_ESPERA_KM = list_create();

	DIC_COLAS_ESPERA_RECURSOS = dictionary_create();
	BLOCK_RECURSO = list_create();

	// Y esta?
	SYSCALLS_COLA = queue_create();

	inicializar_lista_cpu_en_espera();

	inicializar_lista_segmentos_por_hilo();
}

void inicializar_tcb(tcb_t* tcb)
{
	tcb->a = 0;
	tcb->b = 0;
	tcb->c = 0;
	tcb->d = 0;
	tcb->e = 0;
}


void agregar_a_ready(tcb_t* tcb) {
	bloquear_ready();
	list_add(READY_COLA[!tcb->km], tcb);
	desbloquear_ready();

	loggear_estado_de_hilos();

	planificar();
	// Aca deberíamos llamar al planificador. No, no deberiamos. O quizas si, quien lo sabe...
}

bool hay_hilo_km_ready(){
	bloquear_ready();
	int is_empty = list_is_empty(READY_COLA[0]);
	desbloquear_ready();

	return !is_empty;
}

bool hay_hilo_ready(){
	bloquear_ready();
	int is_empty = list_is_empty(READY_COLA[1]);
	desbloquear_ready();

	return !is_empty;
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

	bloquear_exec();
	list_add(EXEC_COLA, ej);
	desbloquear_exec();
}

/*void agregar_a_exit_cola(tcb_t* tcb) {
	list_add(EXIT_COLA, tcb);
}*/

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
	bloquear_exec();
	ejecutando_t* ejecutando = list_remove_by_condition(EXEC_COLA, _igual_pid_tid );
	desbloquear_exec();

	if(ejecutando != NULL)
	{
		tcb_t* tcb_salida = ejecutando->tcb;
		free(ejecutando);
		return tcb_salida;
	}

	return NULL;

	//list_remove_and_destroy_by_condition(EXEC, _igual_pid_tid, destruir_ejecutando);
}

bool esta_ejecutando(uint32_t cpu_id)
{
	bool _cpu_ejecutando(void* e)
	{
		return ((ejecutando_t*) e)->cpu == cpu_id;
	}

	bloquear_exec();
	bool salida = list_any_satisfy(EXEC_COLA, _cpu_ejecutando);
	desbloquear_exec();

	return salida;
}

tcb_t* get_tcb_ejecutando_en_cpu(uint32_t cpu_id)
{
	bool _cpu_ejecutando(void* e)
	{
		return ((ejecutando_t*) e)->cpu == cpu_id;
	}

	bloquear_exec();
	ejecutando_t* ej = list_find(EXEC_COLA, _cpu_ejecutando);
	desbloquear_exec();

	return ej->tcb;
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
		dictionary_put(DIC_COLAS_ESPERA_RECURSOS, recurso, list_create());

	list_add((t_list*)dictionary_get(DIC_COLAS_ESPERA_RECURSOS, recurso), tcb);

	free(recurso);
}

void quitar_de_syscalls_cola(tcb_t* tcb) {
	queue_pop(SYSCALLS_COLA);
}

tcb_t* quitar_primero_de_cola_recurso(uint32_t recurso_int)
{
	char* recurso = identificador_de_recurso(recurso_int);

	// TODO: Modificado testeando (01/12/2014 16:49)
	//tcb_t* tcb = list_get((t_list*)dictionary_get(DIC_COLAS_ESPERA_RECURSOS, recurso), 0);

	tcb_t* tcb = NULL;

	t_list* lista = dictionary_get(DIC_COLAS_ESPERA_RECURSOS, recurso);
	if(lista != NULL)
		tcb = list_get(lista, 0);

	free(recurso);
	return tcb;
}

tcb_t* quitar_de_ready_km(){
	bloquear_ready();
	tcb_t* tcb = list_remove(READY_COLA[0], 0);
	desbloquear_ready();

	return tcb;
}

tcb_t* quitar_de_ready(){
	bloquear_ready();
	tcb_t* tcb = list_remove(READY_COLA[1], 0);
	desbloquear_ready();

	return tcb;
}

char* identificador_de_recurso(uint32_t identificador_int)
{
	char* identificador = malloc(12);

	int i;
	for(i=0;i<12;i++)
		identificador[i] = '\0';

	sprintf(identificador, "%d", identificador_int);

	return identificador;
}

ejecutando_t* buscar_exec_por_pid_tid(uint32_t pid, uint32_t tid)
{
	bool _buscar_pid_tid(void* elemento)
	{
		return ((ejecutando_t*) elemento)->tcb->pid == pid && ((ejecutando_t*) elemento)->tcb->tid == tid;
	}

	bloquear_exec();
	ejecutando_t* ejec = list_find(EXEC_COLA, _buscar_pid_tid);
	desbloquear_exec();

	return ejec;
}

tcb_t* get_tcb_km()
{
	return TCB_KM;
}

bool tcb_km_is_running()
{
	return !list_is_empty(BLOCK_CONCLUSION_KM);
}

bool tcb_km_ocioso()
{
	return !tcb_km_is_running() && !hay_hilo_km_ready();
}

bool hay_hilos_block_espera_km()
{
	return !list_is_empty(BLOCK_ESPERA_KM);
}

void agregar_a_block_espera_km(esperando_km_t* ekm)
{
	list_add(BLOCK_ESPERA_KM, ekm);
}

esperando_km_t* remover_primer_tcb_block_espera_km()
{
	if(!list_is_empty(BLOCK_ESPERA_KM))
		return list_remove(BLOCK_ESPERA_KM, 0);

	return NULL;
}

void agregar_a_block_conclusion_km(tcb_t* tcb)
{
	conclusion_km_t* ckm = malloc(sizeof(conclusion_km_t));
	ckm->tcb = tcb;
	ckm->enviar_a_rdy = true;
	ckm->pid = tcb->pid;

	list_add(BLOCK_CONCLUSION_KM, ckm);
}

conclusion_km_t* get_conclusion_km_t()
{
	return (conclusion_km_t*)list_get(BLOCK_CONCLUSION_KM, 0);
}

void set_enviar_a_rdy(bool un_bool)
{
	conclusion_km_t* ckm = get_conclusion_km_t();
	ckm->enviar_a_rdy = un_bool;
}

tcb_t* get_bloqueado_conclusion_tcb()
{
	if(get_conclusion_km_t() != NULL)
		return (get_conclusion_km_t())->tcb;

	return NULL;
}

void eliminar_conclusion_tcb_sin_quitar_de_exec()
{
	free(get_conclusion_km_t());
	list_remove(BLOCK_CONCLUSION_KM, 0);
}

void eliminar_conclusion_tcb()
{
	if(get_conclusion_km_t() != NULL)
		free(get_conclusion_km_t());

	list_remove(BLOCK_CONCLUSION_KM, 0);

	// Eliminamos el TCB KM de EXEC
	quitar_de_exec(get_tcb_km());
}

void agregar_a_block_join(esperando_join_t* ej)
{
	list_add(BLOCK_JOIN, ej);
}

void notificar_join_finalizacion_hilo(tcb_t* que_termina)
{
	bool _esperando_por(void* elemento)
	{
		return ((esperando_join_t*) elemento)->tcb->pid == que_termina->pid
				&& ((esperando_join_t*) elemento)->esperando_a == que_termina->tid;
	}

	uint32_t cantidad = list_count_satisfying(BLOCK_JOIN, _esperando_por);

	uint32_t i;
	for(i=0; i < cantidad; i++)
	{
		esperando_join_t* ej = list_remove_by_condition(BLOCK_JOIN, _esperando_por);

		agregar_a_ready(ej->tcb);
		free(ej);
	}

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
 * Si la entrada de EXIT es para eliminar un solo Hilo, lo elimina.
 */
void _verificar_salida_proceso(exit_t* et)
{
	if(!et->muere_proceso)
	{// En caso de que un solo TCB muere, lo eliminamos. Si muere el proceso no entra aca
		list_clean_and_destroy_elements(et->lista_tcbs, eliminar_y_destruir_tcb_sin_codigo);
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

	bloquear_ready();
	uint32_t cantidad = list_count_satisfying(READY_COLA[1], _satisface_pid);
	desbloquear_ready();

	int i;
	for(i = 0; i < cantidad; i++)
	{
		agregar_a_exit(list_remove_by_condition(READY_COLA[1], _satisface_pid));
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

/**
 * Elimina un TCB de todas las listas de recursos donde este
 */
void _eliminar_de_listas_recursos(tcb_t* tcb)
{
	// TODO: Habria que desencolar al primero ademas de sacar a este?
	// Lo digo porque me parece que al sacar a este se esta liberando el recurso, pero no se...

	void _eliminar_tcb_cola_recurso(char* key, void* listav)
	{
		t_list* lista = listav;

		bool _satisface_pid_tid(void* elemento)
		{
			return ((tcb_t*) elemento)->pid == tcb->pid &&
					((tcb_t*) elemento)->tid == tcb->tid;
		}

		uint32_t cantidad = list_count_satisfying(lista, _satisface_pid_tid);

		int i;
		for(i = 0; i < cantidad; i++)
		{// Se elimina de la lista, no se agrega a exit porque se agrega en el llamado a esta funcion
			list_remove_by_condition(lista, _satisface_pid_tid);
		}

	}

	dictionary_iterator(DIC_COLAS_ESPERA_RECURSOS, _eliminar_tcb_cola_recurso);
}

void remover_de_block_recursos_a_exit(uint32_t pid)
{
	bool _satisface_pid(void* elemento)
	{
		return ((tcb_t*) elemento)->pid == pid;
	}

	uint32_t cantidad = list_count_satisfying(BLOCK_RECURSO, _satisface_pid);

	int i;
	for(i = 0; i < cantidad; i++)
	{
		tcb_t* tcb = list_remove_by_condition(BLOCK_RECURSO, _satisface_pid);

		_eliminar_de_listas_recursos(tcb);

		agregar_a_exit(tcb);
	}
}

void remover_de_conclusion_km_a_exit(uint32_t pid)
{
	tcb_t* tcb = get_bloqueado_conclusion_tcb();

	if(tcb != NULL)
	{
		if(tcb->pid == pid)
		{
			agregar_a_exit(tcb);

			bloquear_ready();
			int32_t cant = list_size(READY_COLA[0]);
			desbloquear_ready();

			if(cant == 1)
			{// Todavia no entro a ejecutar
				bloquear_ready();
				list_remove(READY_COLA[0], 0);	// Removemos el TCB KM
				desbloquear_ready();

				// Elimina el struct conclusion_km_t
				eliminar_conclusion_tcb();

				// Replanificamos KM para otro TCB
				replanificar_tcb_km();
			}
			else
			{// Ya se esta ejecutando
				eliminar_conclusion_tcb();
			}
		}
	}
}

void eliminar_tcbs_en_exit(uint32_t pid)
{
	bool _buscar_por_pid(void* elemento)
	{
		return ((exit_t*)elemento)->pid == pid;
	}

	exit_t* et = list_find(EXIT_COLA, _buscar_por_pid);

	direccion direccion_codigo = ((tcb_t*)list_get(et->lista_tcbs, 0))->base_codigo;

	// Eliminamos todos los TCBs, pero no el segmento de codigo
	list_clean_and_destroy_elements(et->lista_tcbs, eliminar_y_destruir_tcb_sin_codigo);

	destruir_segmento(pid, direccion_codigo);

	list_destroy(et->lista_tcbs);
	et->lista_tcbs = NULL;
}

void remover_de_exec_a_exit(uint32_t pid)
{
	bool _buscar_por_pid_no_km(void* elemento)
	{
		ejecutando_t* ejec = elemento;
		return ejec->tcb->pid == pid &&
				!ejec->tcb->km;
	}	//TODO: Creo que si es TCB KM hay que sacarlo igual, pero no ponerlo en exit

	bool _buscar_por_pid_y_km(void* elemento)
	{
		ejecutando_t* ejec = elemento;
		return ejec->tcb->pid == pid &&
				ejec->tcb->km;
	}

	bloquear_exec();
	uint32_t cantidad = list_count_satisfying(EXEC_COLA, _buscar_por_pid_no_km);
	desbloquear_exec();

	uint32_t i;
	for(i = 0; i < cantidad; i++)
	{
		bloquear_exec();
		ejecutando_t* et = list_remove_by_condition(EXEC_COLA, _buscar_por_pid_no_km);
		desbloquear_exec();

		agregar_a_exit(et->tcb);

		free(et);
	}
}

bool proceso_muriendo(uint32_t pid)
{
	bool _buscar_por_pid(void* elemento)
	{
		return ((exit_t*)elemento)->pid == pid;
	}

	exit_t* et = list_find(EXIT_COLA, _buscar_por_pid);

	return et != NULL && et->muere_proceso;
}

t_list* get_todos_los_tcbs_exec()
{
	t_list* lista = list_create();

	void _agregar(void* elemento)
	{
		tcb_con_cola_t* tcb_cola = malloc(sizeof(tcb_con_cola_t));
		tcb_cola->tcb = ((ejecutando_t*)elemento)->tcb;
		tcb_cola->cola = MEXEC;
		list_add(lista, tcb_cola);
	}

	bloquear_exec();
	list_iterate(EXEC_COLA, _agregar);
	desbloquear_exec();

	return lista;
}

t_list* get_todos_los_tcbs_rdy()
{
	t_list* lista = list_create();

	void _agregar(void* elemento)
	{
		tcb_con_cola_t* tcb_cola = malloc(sizeof(tcb_con_cola_t));
		tcb_cola->tcb = elemento;
		tcb_cola->cola = MREADY;
		list_add(lista, tcb_cola);
	}

	bloquear_ready();
	list_iterate(READY_COLA[0], _agregar);
	list_iterate(READY_COLA[1], _agregar);
	desbloquear_ready();

	return lista;
}

t_list* get_todos_los_tcbs_block_join()
{
	t_list* lista = list_create();

	void _agregar(void* elemento)
	{
		tcb_con_cola_t* tcb_cola = malloc(sizeof(tcb_con_cola_t));
		tcb_cola->tcb = ((esperando_join_t*)elemento)->tcb;
		tcb_cola->cola = MBLOCK;
		list_add(lista, tcb_cola);
	}

	list_iterate(BLOCK_JOIN, _agregar);

	return lista;
}

t_list* get_todos_los_tcbs_block_espera_km()
{
	t_list* lista = list_create();

	void _agregar(void* elemento)
	{
		tcb_con_cola_t* tcb_cola = malloc(sizeof(tcb_con_cola_t));
		tcb_cola->tcb = ((esperando_km_t*)elemento)->tcb;
		tcb_cola->cola = MBLOCK;
		list_add(lista, tcb_cola);
	}

	list_iterate(BLOCK_ESPERA_KM, _agregar);

	return lista;
}

t_list* get_todos_los_tcbs_block_recurso()
{
	t_list* lista = list_create();

	void _agregar(void* elemento)
	{
		tcb_con_cola_t* tcb_cola = malloc(sizeof(tcb_con_cola_t));
		tcb_cola->tcb = elemento;
		tcb_cola->cola = MBLOCK;
		list_add(lista, tcb_cola);
	}

	list_iterate(BLOCK_RECURSO, _agregar);

	return lista;
}

t_list* get_todos_los_tcbs_exit()
{
	t_list* lista = list_create();

	void _agregar(void* elemento)
	{
		tcb_con_cola_t* tcb_cola = malloc(sizeof(tcb_con_cola_t));
		tcb_cola->tcb = elemento;
		tcb_cola->cola = MEXIT;
		list_add(lista, tcb_cola);
	}

	void _iterar_exit(void* elemento)
	{
		exit_t* ext = elemento;

		if(ext != NULL && ext->lista_tcbs != NULL && !list_is_empty(ext->lista_tcbs))
			list_iterate(ext->lista_tcbs, _agregar);
	}

	list_iterate(EXIT_COLA, _iterar_exit);

	return lista;
}

void unificar_listas_y_eliminar(t_list* principal, t_list* eliminable)
{
	list_add_all(principal, eliminable);
	list_destroy(eliminable);
}

t_list* get_todos_los_tcbs()
{
	t_list* lista_principal = list_create();

	unificar_listas_y_eliminar(lista_principal, get_todos_los_tcbs_exec());

	unificar_listas_y_eliminar(lista_principal, get_todos_los_tcbs_rdy());

	unificar_listas_y_eliminar(lista_principal, get_todos_los_tcbs_block_join());

	if(get_bloqueado_conclusion_tcb() != NULL)
	{
		tcb_con_cola_t* tcb_cola = malloc(sizeof(tcb_con_cola_t));
		tcb_cola->tcb = get_bloqueado_conclusion_tcb();
		tcb_cola->cola = MBLOCK;
		list_add(lista_principal, tcb_cola);
	}

	unificar_listas_y_eliminar(lista_principal, get_todos_los_tcbs_block_espera_km());

	unificar_listas_y_eliminar(lista_principal, get_todos_los_tcbs_block_recurso());

	unificar_listas_y_eliminar(lista_principal, get_todos_los_tcbs_exit());	// TODO: f

	return lista_principal;
}



bool existe_tcb_que_no_termino(uint32_t pid, uint32_t tid){

	bool buscar(void* elemento){
		tcb_con_cola_t* ele = (tcb_con_cola_t*)elemento;
		if (ele->tcb->pid == pid && ele->tcb->tid == tid && ele->cola != MEXIT)
			return	true;
		return false;
	}

	t_list* lista = get_todos_los_tcbs();

	bool existe = list_any_satisfy(lista, buscar);

	list_iterate(lista, free);

	return existe;

}
