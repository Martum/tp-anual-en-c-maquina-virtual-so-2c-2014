/*
 * planificador.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */


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
	rta.flag = 251;

	return rta;
}

void recibir_tcb(pedido_con_resultado_t);

