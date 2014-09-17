/*
 * tcb-funciones.c
 *
 *  Created on: 16/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"
#include "resultados.h"

int actualizar_valor_en_registro(tcb_t* tcb, char* registro, int numero) {
	if (strcmp(registro, "a") == 0) {
		tcb->a = numero;
		return OK;
	}
	if (strcmp(registro, "b") == 0) {
		tcb->b = numero;
		return OK;
	}
	if (strcmp(registro, "c") == 0) {
		tcb->c = numero;
		return OK;
	}
	if (strcmp(registro, "d") == 0) {
		tcb->d = numero;
		return OK;
	}
	if (strcmp(registro, "e") == 0) {
		tcb->e = numero;
		return OK;
	}
	return FALLO;
}

int obtener_valor_de_registro(tcb_t* tcb, char* registro) {
	if (strcmp(registro, "a") == 0) {
		return tcb->a;
	}
	if (strcmp(registro, "b") == 0) {
		return tcb->b;
	}
	if (strcmp(registro, "c") == 0) {
		return tcb->c;
	}
	if (strcmp(registro, "d") == 0) {
		return tcb->d;
	}
	if (strcmp(registro, "e") == 0) {
		return tcb->e;
	}
	return FALLO;
}

int actualizar_pc(tcb_t* tcb, int valor) {
	tcb->pc = valor;
	return OK;
}

int obtener_pc(tcb_t* tcb) {
	return tcb->pc;
}

direccion obtener_base_de_codigo(tcb_t* tcb) {
	return tcb->base_codigo;
}
