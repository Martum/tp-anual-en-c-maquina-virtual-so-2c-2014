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

typedef struct esperando_km {
	tcb_t* tcb;
	uint32_t direccion_syscall;
} esperando_km_t;

typedef struct esperando_join {
	tcb_t* tcb;
	uint32_t esperando_a;
} esperando_join_t;


/**
 * Estructura para la lista de EXIT. Se indica el PID, la lista de TCBs,
 * un flag muere_proceso; que cuando esta en TRUE significa que muere el Proceso entero (todos los Hilos); y
 * se indica la cantidad de Hilos que tiene el proceso.
 */
typedef struct exit {
	uint32_t pid;
	t_list* lista_tcbs;
	uint32_t hilos_totales;
	bool muere_proceso;
} exit_t;

/**
 * Struct para encapsular al TCB que esta esperando al TCB KM que termine
 * de ejecutar la syscall. Si enviar_a_rdy es true (default) se debe encolar en rdy;
 * si es false no se debe encolar ya que esta en alguna cola de bloqueados, solo
 * se deben actualizar los registros.
 */
typedef struct conclusion_km {
	tcb_t* tcb;
	bool enviar_a_rdy;
} conclusion_km_t;

/**
 * Inicializa las colas del Planificador
 */
void inicializar_listas_estados_tcb();

/**
 * Agrega un TCB a la cola de Ready
 */
void agregar_a_ready(tcb_t* tcb);

//void agregar_a_block(tcb_t* tcb);

void agregar_a_exec(tcb_t* tcb, uint32_t cpu_id);

void agregar_a_exit(tcb_t* tcb);

tcb_t* quitar_de_exec(tcb_t* tcb);

//void quitar_de_block(tcb_t* tcb);

void agregar_a_cola_recurso(uint32_t recurso_int, tcb_t* tcb);

tcb_t* quitar_primero_de_cola_recurso(uint32_t recurso_int);

bool hay_hilo_km_ready();

bool hay_hilo_ready();

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

/**
 * Devuelve el TCB que esta bloqueado a la espera de la conclusion del KM
 */
tcb_t* get_bloqueado_conclusion_tcb();

// FUNCIONES BLOCK_RECURSO

/**
 * Agrega a la lista de TCBs bloqueados que estan esperando un Recurso
 */
void agregar_a_block_recurso(tcb_t* tcb);

/**
 * Quita de la lista de TCBs bloqueados que estan esperando un Recurso
 */
void quitar_de_block_recurso(tcb_t* tcb);


// FUNCIONES DE TCB KM

/**
 * Devuelve el TCB KM
 */
tcb_t* get_tcb_km();

/**
 * Verifica si el TCB KM esta corriendo
 * @RETURNS: TRUE si esta en running; FALSE caso contrario
 */
bool tcb_km_is_running();


// FUNCIONES DE BLOCK_ESPERA_KM

/**
 * Verifica si hay hilos en la cola de bloqueados esperando KM libre
 * @RETURNS: TRUE si hay hilos en la cola; false caso contrario
 */
bool hay_hilos_block_espera_km();

/**
 * Agrega a la cola de bloqueados a la espera de KM
 */
void agregar_a_block_espera_km(esperando_km_t* ekm);


// FUNCIONES DE BLOCK_CONCLUSION_KM

/**
 * Agrega a BLOCK_CONCLUSION_KM el TCB
 */
void agregar_a_block_conclusion_km(tcb_t* tcb);

/**
 * Setea el Flag enviar_a_rdy con el valor un_bool
 */
void set_enviar_a_rdy(bool un_bool);


// FUNCIONES DE BLOCK_JOIN

/**
 * Agrega al TCB a la lista de Bloqueados por Join
 */
void agregar_a_block_join(esperando_join_t* ej);


// FUNCIONES DE EXIT Y SIMILARES

/**
 * Agrega un TCB a Exit
 */
void agregar_a_exit(tcb_t* tcb);

/**
 * Saca de READY todos los Hilos con ese PID y los pone en EXIT
 */
void remover_de_ready_a_exit(uint32_t pid);

/**
 * Saca de BLOCK_ESPERA_KM todos los Hilos con ese PID y los pone en EXIT
 */
void remover_de_esperando_km_a_exit(uint32_t pid);

/**
 * Agrega un exit_t a EXIT.
 */
void preparar_exit_para_proceso(uint32_t pid, bool muere_proceso);

/**
 * Agrega a EXIT los TCBs encolados en esperando_km
 */
void remover_de_esperando_km_a_exit(uint32_t pid);

/**
 * Agrega a EXIT los TCBs encolados en Join
 */
void remover_de_join_a_exit(uint32_t pid);

/**
 * Agrega a EXIT los TCBs encolados en Bloqueo por Recurso
 */
void remover_de_block_recursos_a_exit(uint32_t pid);

#endif /* LSTESTADOS_H_ */
