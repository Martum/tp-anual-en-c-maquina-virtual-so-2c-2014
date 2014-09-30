/*
 * tcb-funciones.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include "tcb-funciones.h"

tcb_t* crear_tcb() {
	return (tcb_t*) malloc(sizeof(tcb_t));
}

void liberar_tcb(tcb_t* tcb) {
	free(tcb);
}

resultado_t actualizar_valor_en_registro(tcb_t* tcb, char registro, int32_t numero) {
	if ('a' == registro) {
		tcb->a = numero;
		return OK;
	}
	if ('b' == registro) {
		tcb->a = numero;
		return OK;
	}
	if ('c' == registro) {
		tcb->a = numero;
		return OK;
	}
	if ('d' == registro) {
		tcb->a = numero;
		return OK;
	}
	if ('e' == registro) {
		tcb->a = numero;
		return OK;
	}
	return NO_ENCONTRO_EL_REGISTRO;
}

resultado_t obtener_valor_de_registro(tcb_t* tcb, char registro,
		int32_t* numero) {
	if ('a' == registro) {
		*numero = tcb->a;
		return OK;
	}
	if ('b' == registro) {
		*numero = tcb->b;
		return OK;
	}
	if ('c' == registro) {
		*numero = tcb->c;
		return OK;
	}
	if ('d' == registro) {
		*numero = tcb->d;
		return OK;
	}
	if ('e' == registro) {
		*numero = tcb->e;
		return OK;
	}
	return NO_ENCONTRO_EL_REGISTRO;
}

resultado_t actualizar_pc(tcb_t* tcb, int32_t valor) {
	tcb->pc = valor;
	return OK;
}

resultado_t obtener_pc(tcb_t* tcb) {
	return tcb->pc;
}

direccion obtener_base_de_codigo(tcb_t* tcb) {
	return tcb->base_codigo;
}
