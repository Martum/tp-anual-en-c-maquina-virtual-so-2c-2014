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

int copiar_numero_en_registro(tcb_t* tcb, char* registro, int* numero) {
	if (strcmp(registro, "a") == 0) {
		tcb->a = *numero;
		return 0;
	}
	if (strcmp(registro, "b") == 0) {
		tcb->b = *numero;
		return 0;
	}
	if (strcmp(registro, "c") == 0) {
		tcb->c = *numero;
		return 0;
	}
	if (strcmp(registro, "d") == 0) {
		tcb->d = *numero;
		return 0;
	}
	if (strcmp(registro, "e") == 0) {
		tcb->e = *numero;
		return 0;
	}
	return -1;
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
