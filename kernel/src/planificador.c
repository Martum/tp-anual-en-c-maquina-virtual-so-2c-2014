/*
 * planificador.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include "planificador.h"
#include <hu4sockets/mensajes.h>

#include <stdio.h>
#include <stdlib.h>

#include <commons/collections/list.h>
#include <hu4sockets/sockets.h>
#include <stdbool.h>
#include <pthread.h>
#include "configuraciones.h"
#include "conexiones.h"
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

	flag_t flag = RESPONDO_TCB;

	rta.tcb = _planificar();
	rta.quantum = quantum();
	rta.flag = RESPONDO_TCB;

	return rta;
}

void recibir_tcb(pedido_con_resultado_t resultado){
	if (resultado.flag == TOMA_RESULTADO)
}

