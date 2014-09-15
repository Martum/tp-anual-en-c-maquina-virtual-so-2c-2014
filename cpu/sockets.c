/*
 * memoria.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */
#include <stdio.h>
#include "estructuras.h"

int conectar_con_memoria() {
	printf("Me conecto con la memoria MSP. Si falla la conexion devuelvo -1\n");
	return 0;
}

int conectar_con_kernel() {
	printf("Me conecto con el Kernel. Si falla la conexion devuelvo -1\n");
	return 0;
}

int pedir_tcb(tcb_t tcb, int* quantum) {
	printf("Mientras no me de un tcb devuelvo -1");
	return 0;
}

int leer_de_memoria(direccion pc, int bytes, char* buffer) {
	// tendria que devolver la cantidad de byes que leyo o algo asi
	return 0;
}
