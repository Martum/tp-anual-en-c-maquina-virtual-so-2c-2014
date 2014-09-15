/*
 * serviciosacpu.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */


#include "loader.h"
#include "lstestados.h"
//#include "serviciosacpu.h"

int crear_hilo(tcb_t* tcb, uint32_t tid)
{
	tcb_t* tcb_hilo = crear_tcb(tcb->pid, tid);

	if(cargar_tcb_sin_codigo(tcb, tcb_hilo) != -1)
	{
		agregar_a_ready(tcb_hilo);
		return 0;
	}

	return -1;
}
