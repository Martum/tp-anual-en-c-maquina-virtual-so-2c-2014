/*
 * syscalls.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */


void recibir_syscall(tcb_t* tcb, direccion dir){
	// FALTA TENER EN CUENTA LA DIRECCION. CREAR NUEVA ESTRUCTURA??
	quitar_de_exec(tcb);
	agregar_a_block(tcb);
	agregar_a_syscalls(tcb);
}
