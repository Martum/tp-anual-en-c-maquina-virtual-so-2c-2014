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

t_queue* cpu_en_espera_de_tcb = NULL;

void agregar_a_cpu_en_espera_de_tcb(uint32_t cpu_id) {
	if (cpu_en_espera_de_tcb == NULL ) {
		cpu_en_espera_de_tcb = queue_create();
	}

	uint32_t* cpu = malloc(sizeof(uint32_t));
	*cpu = cpu_id;
	queue_push(cpu_en_espera_de_tcb, cpu);
}

void* quitar_de_cpu_en_espera_de_tcb() {
	return queue_pop(cpu_en_espera_de_tcb);
}

// TODO: FALTA CONTEMPLAR EL CASO QUE NO HAYA PROXIMO TCB
//tcb_t* _proximo_tcb(uint32_t cpu_id)
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

// TODO: el planificador tiene que manejar una cola de cpus en espera de tcb. Cuando le llega una peticion
// de tcb, encola  la cpu en la lista y corre el planificador. Cuando un tcb entra en ready,
//debe revisar si hay alguna cpu en espera, si la hay le da el tcb.

void pedir_tcb(uint32_t cpu_id) {
	agregar_a_cpu_en_espera_de_tcb(cpu_id);
	planificar();
}

void planificar() {
	if (!queue_is_empty(cpu_en_espera_de_tcb)) {
		tcb_t* tcb = _proximo_tcb();
		if (tcb != NULL ) {
			uint32_t* cpu_id = quitar_de_cpu_en_espera_de_tcb();
			_enviar_tcb_a_cpu(tcb, cpu_id);
			agregar_a_exec(tcb, *cpu_id);
		}
	}

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

// FALTA EL WRAPPER QUE DESERIALICE. YO VOY A RECIBIR UN CHORRO DE BYTES QUE TENGO
// QUE TRANSFORMAR EN RESULTADO_T Y TCB_T. ESA FUNCION VA A ESTAR EN UN SUPER CASE EN CONEXIONES.C
void recibir_tcb(resultado_t resultado, tcb_t* tcb) {

	tcb_t* tcb_posta = quitar_de_exec(tcb);
	if (tcb->km) {
		copiar_registros_programacion(tcb_posta, tcb);
	} else {
		copiar_tcb(tcb_posta, tcb);
	}

	switch (resultado) {
	case FIN_QUANTUM:
		agregar_a_ready(tcb_posta);
		break;

	case ERROR_EN_EJECUCION:
		mover_tcbs_a_exit(tcb_posta->pid);
		break;

	case FIN_EJECUCION:
		notificar_join_finalizacion_hilo(tcb_posta);
		if (tcb->km) {
			agregar_a_ready(tcb_posta);
		} else {
			agregar_a_exit(tcb_posta);
		}
		break;

	default:
		break;

	}
}

void mover_tcbs_a_exit(uint32_t pid) {
	preparar_exit_para_proceso(pid, true);

	remover_de_ready_a_exit(pid);

	remover_de_exec_a_exit(pid);

	// TODO: Verificar la cola rdy del KM
	// Si el KM en rdy es de un hilo de este proceso, sacarlo y replanificarlo.
	// Antes de eso recordar que el tcb en block_conclusion_km es el que hay que mnadar a exit

	remover_de_esperando_km_a_exit(pid);

	remover_de_conclusion_km_a_exit(pid);	// VER QUE ELIMINA Y QUE NO ESTA FUNCION -> LINEA 462 CONEXIONES.C

	remover_de_join_a_exit(pid);

	remover_de_block_recursos_a_exit(pid);// Es de la lista de bloqueados y de las del diccionario

	// TODO: Destruir los Segmentos que van a estar en la lista de segmentos por hilo

	eliminar_tcbs_en_exit(pid);			// Eliminamos los TCBs definitivamente
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
