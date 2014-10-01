/*
 * planificador.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include "planificador.h"
#include <hu4sockets/mensajes.h>
#include <hu4sockets/tcb.h>


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

// FALTA EL WRAPPER QUE LLAME A ESTA FUNCION Y LE MANDE A LA CPU LO QUE RETORNA ESTO.
respuesta_de_nuevo_tcb_t  _proximo_tcb(){

	//TODO: Cuidado aca, no se como lo usas, pero en el 99% de los casos
	// las variables tienen que crearse con malloc

	respuesta_de_nuevo_tcb_t rta;

	rta.tcb = _planificar();
	rta.quantum = quantum();
	rta.flag = TOMA_TCB;

	return rta;
}

// FALTA EL WRAPPER QUE SERIALIZE. YO VOY A RECIBIR UN CHORRO DE BYTES QUE TENGO
// QUE TRANSFORMAR EN RESULTADO_T Y TCB_T
void recibir_tcb(resultado_t resultado, tcb_t* tcb){
	quitar_de_exec(tcb);

	switch(resultado){
	 	case FIN_QUANTUM:
	 		agregar_a_ready(tcb);_	//TODO: y este guion bajo?
	 		break;

	 	case BLOCK:
	 		agregar_a_block(tcb);
	 		break;

	 	case DESCONEXION_CPU:
	 	case ERROR:
	 	case FIN_EJECUCION:
	 		agregar_a_exit(tcb);
	 		break;

	}
}

