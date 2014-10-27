/*
 * tcb-funciones.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include "tcb-funciones.h"

tcb_t* crear_tcb()
{
	return malloc(sizeof(tcb_t));
}

void liberar_tcb(tcb_t* tcb)
{
	free(tcb);
}

void clonar_tcb(tcb_t* destino, tcb_t* fuente)
{
	memcpy(destino, fuente, sizeof(tcb_t));
}

bool es_tcb_kernel(tcb_t* tcb)
{
	return tcb->km;
}

uint32_t obtener_ocupacion_stack(tcb_t* tcb)
{
	return tcb->cursor_stack - tcb->base_stack;
}

resultado_t actualizar_valor_del_registro(tcb_t* tcb, char registro,
	int32_t numero)
{
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
	int32_t* numero)
{
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

void actualizar_pc(tcb_t* tcb, direccion nuevo_pc)
{
	tcb->pc = nuevo_pc;
}

void actualizar_tid(tcb_t* tcb, int32_t nuevo_tid)
{
	tcb->km = nuevo_tid;
}

void actualizar_km(tcb_t* tcb, bool nuevo_km)
{
	tcb->km = nuevo_km;
}

void actualizar_base_del_stack(tcb_t* tcb, direccion nueva_base)
{
	tcb->base_stack = nueva_base;
}

void actualizar_registro_a(tcb_t* tcb, int32_t nuevo_valor)
{
	tcb->a = nuevo_valor;
}

void actualizar_registro_b(tcb_t* tcb, int32_t nuevo_valor)
{
	tcb->b = nuevo_valor;
}

direccion obtener_base_de_codigo(tcb_t* tcb)
{
	return tcb->base_codigo;
}

int32_t obtener_valor_registro_a(tcb_t* tcb)
{
	return tcb->a;
}

int32_t obtener_valor_registro_b(tcb_t* tcb)
{
	return tcb->b;
}

resultado_t mover_cursor_stack(tcb_t* tcb, int32_t cantidad_de_bytes)
{
	if (tcb->base_stack < tcb->cursor_stack + cantidad_de_bytes)
		return EXCEPCION_POR_POSICION_DE_STACK_INVALIDA;

	tcb->cursor_stack = tcb->cursor_stack + cantidad_de_bytes;

	return OK;
}
