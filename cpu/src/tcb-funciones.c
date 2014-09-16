/*
 * tcb-funciones.c
 *
 *  Created on: 16/09/2014
 *      Author: utnso
 */

#include <stdlib.h>
#include <string.h>
#include "estructuras.h"

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
	return -1;
}
