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

tcb_t* _proximo_tcb(){

	tcb_t* tcb;
	if (hay_hilo_km_ready()){
		tcb = quitar_de_ready_km();
		agregar_a_exec(tcb);
	}else{
		tcb = quitar_de_ready();
		agregar_a_exec(tcb);
	}

	return tcb;
}


// FALTA EL FREE
char*  rta_nuevo_tcb(){

	respuesta_de_nuevo_tcb_t* rta = malloc(sizeof(respuesta_de_nuevo_tcb_t));

	tcb_t* tcb = _proximo_tcb();
	rta->tcb = tcb;
	rta->quantum = quantum();
	rta->flag = TOMA_TCB;

	return serializar_respuesta_de_nuevo_tcb_t(rta);
}

// FALTA EL WRAPPER QUE DESERIALIZE. YO VOY A RECIBIR UN CHORRO DE BYTES QUE TENGO
// QUE TRANSFORMAR EN RESULTADO_T Y TCB_T. ESA FUNCION VA A ESTAR EN HU4SOCKETS
// MENSAJES.C
void recibir_tcb(resultado_t resultado, tcb_t* tcb){
	quitar_de_exec(tcb);

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

