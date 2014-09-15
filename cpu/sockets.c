/*
 * memoria.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"

int conectar_con_memoria() {
	printf("Me conecto con la memoria MSP. Si falla la conexion devuelvo -1\n");
	return 0;
}

int conectar_con_kernel() {
	printf("Me conecto con el Kernel. Si falla la conexion devuelvo -1\n");
	return 0;
}

int pedir_tcb(tcb_t* tcb, int* quantum) {
	printf("Mientras no me de un tcb devuelvo -1");
	return 0;
}

int leer_de_memoria(direccion dir, int bytes, void* buffer) {
	// tendria que devolver la cantidad de byes que leyo o algo asi
	return 0;
}

int informar_a_kernel_de_finalizacion(tcb_t* tcb, int res) {
	// deberia mandar al Kernel el tcb modificado y porque finalizo
	return 0;
}

char* obtener_registro(tcb_t* tcb) {
	char* registro = malloc(sizeof(char));
	leer_de_memoria(tcb->pc, sizeof(char), registro);
	tcb->pc = tcb->pc + 1;
	return registro;
}

int* obtener_numero(tcb_t* tcb) {
	int* numero = malloc(sizeof(int));
	leer_de_memoria(tcb->pc, sizeof(int), numero);
	tcb->pc = tcb->pc + 4;
	return numero;
}

void copiar_numero_en_registro(tcb_t* tcb, char* registro, int* numero) {
	if (strcmp(registro, "a") == 0) {
		tcb->a = (int) &numero;
	}
	if (strcmp(registro, "b") == 0) {
		tcb->b = (int) &numero;
	}
	if (strcmp(registro, "c") == 0) {
		tcb->c = (int) &numero;
	}
	if (strcmp(registro, "d") == 0) {
		tcb->d = (int) &numero;
	}
	if (strcmp(registro, "e") == 0) {
		tcb->e = (int) &numero;
	}
}
