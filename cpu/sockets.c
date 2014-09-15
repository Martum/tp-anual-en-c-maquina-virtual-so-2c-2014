/*
 * memoria.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */
#include <stdio.h>

int conectar_con_memoria() {
	printf("Me conecto con la memoria MSP. Si falla la conexion devuelvo -1\n");
	return 0;
}

int conectar_con_kernel() {
	printf("Me conecto con el Kernel. Si falla la conexion devuelvo -1\n");
	return 0;
}

int pido_tcb(tcb_t tcb, int* quantum) {
	printf("Mientras no me de un tcb devuelvo -1");
	return 0;
}
