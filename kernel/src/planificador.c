/*
 * planificador.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include "planificador.h"
#include <hu4sockets/mensajes.h>

tcb_t* _planificar(){

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

respuesta_de_nuevo_tcb_t  devolver_tcb(){
	respuesta_de_nuevo_tcb_t rta;

	rta.tcb = _planificar();
	rta.quantum = quantum();
	rta.flag = TOMA_TCB;

	return rta;
}

void recibir_tcb(pedido_con_resultado_t resultado){
	switch(resultado.resultado){
	 	case FIN_QUANTUM:
	 		break;

	 	case INTERRUPCION:
	 		break;

	 	case DESCONEXION_CONSOLA:
	 		break;

	 	case DESCONEXION_CPU:
	 		break;

	}
}

