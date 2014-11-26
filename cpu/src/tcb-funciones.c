/*
 * tcb-funciones.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include "tcb-funciones.h"

bool es_tcb_kernel(tcb_t* tcb)
{
	return tcb->km;
}

resultado_t actualizar_valor_del_registro(tcb_t* tcb, char registro,
	int32_t numero)
{
	loggear_debug("Actualizo el valor del registro %c", registro);

	if ('A' == registro)
	{
		loggear_debug("Valor viejo %d -> Valor nuevo %d", tcb->a, numero);
		ansisop_cambio_registro_a(numero);
		tcb->a = numero;
		return OK;
	}
	if ('B' == registro)
	{
		loggear_debug("Valor viejo %d -> Valor nuevo %d", tcb->b, numero);
		ansisop_cambio_registro_b(numero);
		tcb->b = numero;
		return OK;
	}
	if ('C' == registro)
	{
		loggear_debug("Valor viejo %d -> Valor nuevo %d", tcb->c, numero);
		ansisop_cambio_registro_c(numero);
		tcb->c = numero;
		return OK;
	}
	if ('D' == registro)
	{
		loggear_debug("Valor viejo %d -> Valor nuevo %d", tcb->d, numero);
		ansisop_cambio_registro_d(numero);
		tcb->d = numero;
		return OK;
	}
	if ('E' == registro)
	{
		loggear_debug("Valor viejo %d -> Valor nuevo %d", tcb->e, numero);
		ansisop_cambio_registro_e(numero);
		tcb->e = numero;
		return OK;
	}

	loggear_warning("No existe el registro %c, byte %x", registro, registro);

	return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
}

resultado_t obtener_valor_del_registro(tcb_t* tcb, char registro,
	int32_t* numero)
{
	loggear_debug("Obtengo valor de registro %c", registro);

	if ('A' == registro)
	{
		loggear_debug("Valor del registro %c es %d", registro, tcb->a);
		*numero = tcb->a;
		return OK;
	}
	if ('B' == registro)
	{
		loggear_debug("Valor del registro %c es %d", registro, tcb->b);
		*numero = tcb->b;
		return OK;
	}
	if ('C' == registro)
	{
		loggear_debug("Valor del registro %c es %d", registro, tcb->c);
		*numero = tcb->c;
		return OK;
	}
	if ('D' == registro)
	{
		loggear_debug("Valor del registro %c es %d", registro, tcb->d);
		*numero = tcb->d;
		return OK;
	}
	if ('E' == registro)
	{
		loggear_debug("Valor del registro %c es %d", registro, tcb->e);
		*numero = tcb->e;
		return OK;
	}

	loggear_warning("No existe el registro %c, byte %x", registro, registro);

	return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
}

void actualizar_pc(tcb_t* tcb, direccion nuevo_pc)
{
	loggear_trace("Actualizo valor del pc");
	loggear_trace("Vieja pc %x -> Nueva pc %x", tcb->pc, nuevo_pc);
	ansisop_cambio_registro_pc(nuevo_pc);
	tcb->pc = nuevo_pc;
}

void actualizar_registro_a(tcb_t* tcb, int32_t nuevo_valor)
{
	loggear_debug("Actualizo registro A");
	loggear_debug("Viejo valor %d -> Nuevo valor %d", tcb->a, nuevo_valor);
	ansisop_cambio_registro_a(nuevo_valor);
	tcb->a = nuevo_valor;
}

void actualizar_registro_b(tcb_t* tcb, int32_t nuevo_valor)
{
	loggear_debug("Actualizo registro B");
	loggear_debug("Viejo valor %d -> Nuevo valor %d", tcb->b, nuevo_valor);
	ansisop_cambio_registro_b(nuevo_valor);
	tcb->b = nuevo_valor;
}

direccion obtener_base_de_codigo(tcb_t* tcb)
{
	return tcb->base_codigo;
}

int32_t obtener_valor_registro_a(tcb_t* tcb)
{
	loggear_debug("Obtengo el valor del registro A");
	loggear_debug("Valor %d", tcb->a);
	return tcb->a;
}

int32_t obtener_valor_registro_b(tcb_t* tcb)
{
	loggear_debug("Obtengo el valor del registro B");
	loggear_debug("Valor %d", tcb->b);
	return tcb->b;
}

resultado_t mover_cursor_stack(tcb_t* tcb, int32_t cantidad_de_bytes)
{
	loggear_debug("Muevo cursor del stack");
	loggear_debug("Posicion actual %x -- Bytes a mover %d", tcb->cursor_stack, cantidad_de_bytes);
	if (tcb->base_stack > tcb->cursor_stack + cantidad_de_bytes) {
		loggear_warning("La cantidad de bytes a mover sobrepasa la base");
		return EXCEPCION_POR_POSICION_DE_STACK_INVALIDA;
	}

	tcb->cursor_stack = tcb->cursor_stack + cantidad_de_bytes;

	ansisop_cambio_registro_cursor(tcb->cursor_stack);
	loggear_debug("Base stack %x -- Curso stack %x", tcb->base_stack, tcb->cursor_stack);

	return OK;
}
