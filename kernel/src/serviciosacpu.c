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


