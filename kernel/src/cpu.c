/*
 * serviciosacpu.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */


#include "loader.h"
#include "lstestados.h"
#include "consola.h"
#include "cpu.h"

uint32_t ID_CPU_GLOBAL = 1;

//TODO: Comentado para que compile
/*
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
*/

uint32_t dame_nuevo_id_cpu()
{
	return ID_CPU_GLOBAL++;
}

void enviar_entrada_estandar(uint32_t pid, uint16_t identificador_tipo){
	//REVISAR. Utilizar el enviar y recibir para evitar espera activa.
	if (identificador_tipo == 1){
		pedido_leer_int(pid);
	}
	if (identificador_tipo == 2){
		pedido_leer_cadena(pid);
	}
}
/*
void* recibir_entrada_estandar(uint32_t pid, void* ){
	codear;
}

int salida_estandar(uint32_t pid, char* cadena){
	return escribir(pid, cadena);
}
*/

void bloquear(tcb_t* tcb, uint32_t recurso){
	quitar_de_exec(tcb);
	agregar_a_block(tcb);
	agregar_a_cola_recurso(recurso, tcb);
}

void despertar(uint32_t recurso){
	tcb_t* tcb = quitar_primero_de_cola_recurso(recurso);
	quitar_de_block(tcb);
	agregar_a_ready(tcb);
}



