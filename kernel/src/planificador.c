/*
 * planificador.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include "planificador.h"
#include "configuraciones.h"
#include "lstestados.h"
#include <hu4sockets/mensajes.h>
#include <hu4sockets/tcb.h>
#include <stdlib.h>

tcb_t* _proximo_tcb(uint32_t cpu_id)
{

	tcb_t* tcb = NULL;
	if (hay_hilo_km_ready())
	{
		tcb = quitar_de_ready_km();
		agregar_a_exec(tcb, cpu_id);
	}
	else
	{
		tcb = quitar_de_ready();
		agregar_a_exec(tcb, cpu_id);
	}

	return tcb;
}


// TODO: FALTA EL FREE. Vos... el que me llama... hacelo!
char*  rta_nuevo_tcb(uint32_t cpu_id){

	respuesta_de_nuevo_tcb_t* rta = malloc(sizeof(respuesta_de_nuevo_tcb_t));

	tcb_t* tcb = _proximo_tcb(cpu_id);
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
void recibir_tcb(resultado_t resultado, tcb_t* tcb){
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

