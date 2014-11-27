/*
 * planificador.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include "planificador.h"
#include "configuraciones.h"
#include "conexiones.h"
#include "lstestados.h"
#include <hu4sockets/mensajes.h>
#include <hu4sockets/sockets.h>
#include <hu4sockets/tcb.h>
#include <stdlib.h>
#include "memoria.h"
#include "cpu.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

// MUTEX para garantizar unicidad y atomicidad de planificar
pthread_mutex_t PLANIFICANDO = PTHREAD_MUTEX_INITIALIZER;

t_list* CPU_EN_ESPERA_DE_TCB = NULL;


void bloquear_planificar()
{
	pthread_mutex_lock(&PLANIFICANDO);
}

void desbloquear_planificar()
{
	pthread_mutex_unlock(&PLANIFICANDO);
}

void inicializar_lista_cpu_en_espera()
{
	CPU_EN_ESPERA_DE_TCB = list_create();
}

void agregar_a_cpu_en_espera_de_tcb(uint32_t cpu_id)
{
	uint32_t* cpu = malloc(sizeof(uint32_t));
	*cpu = cpu_id;
	list_add(CPU_EN_ESPERA_DE_TCB, cpu);
}

void* quitar_de_cpu_en_espera_de_tcb() {
	return list_remove(CPU_EN_ESPERA_DE_TCB, 0);
}

void quitar_cpu_de_lista_espera_tcb(uint32_t cpu_id)
{
	bool _cpu_por_id(void* e)
	{
		return *((uint32_t*)e) == cpu_id;
	}

	list_remove_and_destroy_by_condition(CPU_EN_ESPERA_DE_TCB, _cpu_por_id, free);
}


tcb_t* _proximo_tcb() {
	tcb_t* tcb = NULL;

	if (tcb_km_ocioso() && hay_hilos_block_espera_km())
		replanificar_tcb_km();

	if (hay_hilo_km_ready()) {
		tcb = quitar_de_ready_km();
		//agregar_a_exec(tcb, cpu_id);
	} else {
		tcb = quitar_de_ready();
		/*if (tcb != NULL){ // Por si no hay tcb en ready.
		 agregar_a_exec(tcb, cpu_id);
		 }*/
	}

	return tcb;
}

void pedir_tcb(uint32_t cpu_id) {
	agregar_a_cpu_en_espera_de_tcb(cpu_id);
	planificar();
}

void planificar() {
	bloquear_planificar();

	replanificar_tcb_km();

	if (!list_is_empty(CPU_EN_ESPERA_DE_TCB)) {
		tcb_t* tcb = _proximo_tcb();
		if (tcb != NULL ) {
			uint32_t* cpu_id = quitar_de_cpu_en_espera_de_tcb();
			_enviar_tcb_a_cpu(tcb, cpu_id);
			agregar_a_exec(tcb, *cpu_id);
		}
	}

	desbloquear_planificar();
}

void _enviar_tcb_a_cpu(tcb_t* tcb, uint32_t* cpu_id) {
	uint32_t tamanio = tamanio_respuesta_de_nuevo_tcb_t_serializado();

	char* respuesta = malloc(tamanio);
	respuesta = _rta_nuevo_tcb(cpu_id, tcb);

	sock_t* socket = buscar_conexion_cpu_por_id(*cpu_id);
	enviar(socket, respuesta, &tamanio);

	free(respuesta);
}

// TODO: FALTA EL FREE. Vos... el que me llama... hacelo!
char* _rta_nuevo_tcb(uint32_t cpu_id, tcb_t* tcb) {

	respuesta_de_nuevo_tcb_t* rta = malloc(sizeof(respuesta_de_nuevo_tcb_t));

	rta->tcb = tcb;			// No hacer free de esto eh!
	rta->quantum = quantum();
	rta->flag = TOMA_TCB;

	char* salida = serializar_respuesta_de_nuevo_tcb_t(rta);
	free(rta);
	return salida;
}


void recibir_tcb(resultado_t resultado, tcb_t* tcb) {

	tcb_t* tcb_posta = quitar_de_exec(tcb);
	if (tcb->km)
	{
		tcb_posta = ((conclusion_km_t*)get_conclusion_km_t())->tcb;
		copiar_registros_programacion(tcb_posta, tcb);
	}
	else
	{
		copiar_tcb(tcb_posta, tcb);
	}

	switch (resultado) {
	case FIN_QUANTUM:
		agregar_a_ready(tcb_posta);
		break;

	case ERROR_EN_EJECUCION:
		/*if(tcb->km)
			eliminar_conclusion_tcb();*/	//TODO: Creo que este codigo no va

		mover_tcbs_a_exit_posta(tcb_posta->pid, tcb_posta, true);
		break;


	case FIN_EJECUCION:
		if (tcb->km)
		{
			conclusion_km_t* ckm = get_conclusion_km_t();

			if(ckm->enviar_a_rdy)
				agregar_a_ready(tcb_posta);

			eliminar_conclusion_tcb_sin_quitar_de_exec();
		}
		else
		{
			if(tcb_posta->tid == 1)
			{// Muere proceso por ser Hilo principal
				mover_tcbs_a_exit_posta(tcb_posta->pid, tcb_posta, true);
			}
			else
			{
				notificar_join_finalizacion_hilo(tcb_posta);

				preparar_exit_para_proceso(tcb_posta->pid, false);
				destruir_segmentos_de_hilo(tcb_posta->pid, tcb_posta->tid);
				agregar_a_exit(tcb_posta);
			}
		}
		break;

	default:
		break;

	}
}



void mover_tcbs_a_exit_posta(uint32_t pid, tcb_t* tcb_adicional, bool desconectar_consola)
{
	preparar_exit_para_proceso(pid, true);

	remover_de_ready_a_exit(pid);

	remover_de_exec_a_exit(pid);

	remover_de_esperando_km_a_exit(pid);

	remover_de_conclusion_km_a_exit(pid);	// VER QUE ELIMINA Y QUE NO ESTA FUNCION -> LINEA 462 CONEXIONES.C

	remover_de_join_a_exit(pid);

	remover_de_block_recursos_a_exit(pid);// Es de la lista de bloqueados y de las del diccionario

	if(tcb_adicional != NULL)
		agregar_a_exit(tcb_adicional);

	eliminar_tcbs_en_exit(pid);			// Eliminamos los TCBs definitivamente

	destruir_segmentos_de_proceso(pid);

	if(desconectar_consola)
	{
		enviar_desconectate(buscar_conexion_proceso_por_pid(pid));
		eliminar_conexion_proceso(buscar_conexion_proceso_por_pid(pid));
	}
}

void mover_tcbs_a_exit(uint32_t pid, bool desconectar_consola)
{
	mover_tcbs_a_exit_posta(pid, NULL, desconectar_consola);
}

void eliminar_y_destruir_tcb_sin_codigo(void* tcbv) {
	tcb_t* tcb = tcbv;

	destruir_segmento(tcb->pid, tcb->base_stack);

	free(tcb);
}

void eliminar_y_destruir_tcb(void* tcbv) {
	tcb_t* tcb = tcbv;

	destruir_segmento(tcb->pid, tcb->base_codigo);
	destruir_segmento(tcb->pid, tcb->base_stack);

	free(tcb);
}

void replanificar_tcb_km()
{
	if(tcb_km_ocioso())
	{
		esperando_km_t* ekm = remover_primer_tcb_block_espera_km();

		if(ekm!= NULL)
		{
			preparar_km_para_ejecutar(ekm->tcb, ekm->direccion_syscall);

			free(ekm);
		}
	}
}
