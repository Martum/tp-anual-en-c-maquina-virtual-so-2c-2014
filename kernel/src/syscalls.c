/*
 * syscalls.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include "syscall.h"
#include <hu4sockets/mensajes.h>
#include "lstestados.h"

void recibir_syscall(tcb_t* tcb, direccion dir){
	// poner la dir en tcb.pc
	quitar_de_exec(tcb);
	agregar_a_block(tcb);

	// TODO: Comentado para que compile, hay que solucionarlo
	//agregar_a_syscalls(tcb);
}
