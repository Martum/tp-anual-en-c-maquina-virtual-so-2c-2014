/*
 * serviciosacpu.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */


#include "loader.h"
#include "lstestados.h"
#include "consola.h"
//#include "serviciosacpu.h"

int crear_hilo(tcb_t* tcb, uint32_t tid)
{
	// Creamos TCB
	tcb_t* tcb_hilo = crear_tcb(tcb->pid, tid);

	// Le asignamos la misma base_codigo y un nuevo base_stack
	if(cargar_tcb_sin_codigo(tcb, tcb_hilo) != -1)
	{// Hay memoria, agregamos a RDY
		agregar_a_ready(tcb_hilo);
		return 0;
	}

	return -1;
}

int entrada_estandar(uint32_t pid, uint16_t identificador_tipo){
	//obtener consola correspondiente al pid recibido.
	if (identificador_tipo == 1){
		//leer_int() a la consola.
	}
	if (identificador_tipo == 2){
		//leer_cadena() a la consola.
	}

}

int salida_estandar(uint32_t pid, char* cadena){
	//obtener consola correspondiente al pid recibido.
	//escribir(cadena) a la consola.
}


int bloquear(tcb_t* tcb, uint32_t recurso){
	quitar_de_exec(tcb);
	agregar_a_block(tcb);
	agregar_a_cola_recurso(recurso, tcb);
}

int despertar(uint32_t recurso){
	tcb_t* tcb = quitar_primero_de_cola_recurso(recurso);
	quitar_de_block(tcb);
	agregar_a_ready(tcb);
}



