/*
 * memoria.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "estructuras.h"
#include "resultados.h"

int32_t conectar_con_memoria() {
	// Me conecto con la memoria MSP. Si falla la conexion devuelvo -1
	return 0;
}

int32_t conectar_con_kernel() {
	// "Me conecto con el Kernel. Si falla la conexion devuelvo -1
	return 0;
}

int32_t pedir_tcb(tcb_t* tcb, int32_t* quantum) {
	// Mientras no me de un tcb devuelvo -1
	return 0;
}

direccion crear_segmento(tcb_t* tcb, int32_t bytes) {
	return 0;
}

int32_t destruir_segmento(tcb_t* tcb, direccion direccion) {
	return 0;
}

int32_t leer_de_memoria(direccion dir, int32_t bytes, void* buffer) {
	// tendria que devolver la cantidad de byes que leyo o algo asi
	return 0;
}

int32_t escribir_en_memoria(direccion dir, int32_t byes, void* buffer) {
	// tendria que devolver la cantidad de byes que leyo o algo asi
	return 0;
}

int32_t informar_a_kernel_de_finalizacion(tcb_t* tcb, resultado_t res) {
	// deberia mandar al Kernel el tcb modificado y porque finalizo
	return 0;
}

char obtener_registro(tcb_t* tcb) {
	char* registro = malloc(sizeof(char));
	leer_de_memoria(tcb->pc, sizeof(char), registro);
	tcb->pc = tcb->pc + 1;
	return *registro;
}

int32_t obtener_numero(tcb_t* tcb) {
	int32_t* numero = malloc(sizeof(int32_t));
	leer_de_memoria(tcb->pc, sizeof(int32_t), numero);
	tcb->pc = tcb->pc + 4;
	return *numero;
}
