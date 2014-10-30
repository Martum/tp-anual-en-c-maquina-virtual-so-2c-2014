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
	_planificar();
}

void _planificar() {
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
}

// TODO: FALTA EL FREE. Vos... el que me llama... hacelo!
char* _rta_nuevo_tcb(uint32_t cpu_id, tcb_t* tcb) {

	respuesta_de_nuevo_tcb_t* rta = malloc(sizeof(respuesta_de_nuevo_tcb_t));

	//tcb_t* tcb = _proximo_tcb(cpu_id);
	rta->tcb = tcb;			// No hacer free de esto eh!
	rta->quantum = quantum();
	rta->flag = TOMA_TCB;

	char* salida = serializar_respuesta_de_nuevo_tcb_t(rta);
	free(rta);
	// TODO: Y si no hay TCBs en RDY que hacemos??? BUG!
	return salida;
}

// FALTA EL WRAPPER QUE DESERIALICE. YO VOY A RECIBIR UN CHORRO DE BYTES QUE TENGO
// QUE TRANSFORMAR EN RESULTADO_T Y TCB_T. ESA FUNCION VA A ESTAR EN UN SUPER CASE EN CONEXIONES.C
void recibir_tcb(resultado_t resultado, tcb_t* tcb) {
	quitar_de_exec(tcb);
// TODO: OJO que esto ya no es tan asi. Ver bien entre todos
	/*
	 *
	 switch(resultado){
	 case FIN_QUANTUM:
	 agregar_a_ready(tcb);
	 break;

	 case BLOCK:
	 agregar_a_block(tcb);
	 break;

	 case DESCONEXION_CPU:
	 case ERROR:
	 agregar_a_exit(tcb);
	 break;
	 case FIN_EJECUCION:
	 // TODO: Recordar que hay que verificar los TCBs bloqueados con JOIN
	 agregar_a_exit(tcb);
	 break;
	 default:
	 break;

	 } */
}

