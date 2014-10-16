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

resultado_t actualizar_valor_del_registro(tcb_t* tcb, char registro, int32_t numero) {
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
	return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
}

resultado_t obtener_valor_del_registro(tcb_t* tcb, char registro,
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
	return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
}

void actualizar_pc(tcb_t* tcb, direccion direccion) {
	tcb->pc = direccion;
}

void actualizar_cursor_stack(tcb_t* tcb, int32_t cantidad_de_bytes) {
	tcb->cursor_stack = tcb->cursor_stack + cantidad_de_bytes; // todo pensar si no debe devolver FALLO si es menor a base
}

void obtener_pc(tcb_t* tcb, direccion* pc) {
	*pc = tcb->pc;
}

void obtener_base_de_codigo(tcb_t* tcb, direccion* base) {
	*base = tcb->base_codigo;
}
