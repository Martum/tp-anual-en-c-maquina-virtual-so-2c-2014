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
	loggear_trace("Actualizo el valor del registro %c", registro);

	if ('a' == registro)
	{
		loggear_trace("Valor viejo %d -> Valor nuevo %d", tcb->a, numero);
		tcb->a = numero;
		return OK;
	}
	if ('b' == registro)
	{
		loggear_trace("Valor viejo %d -> Valor nuevo %d", tcb->b, numero);
		tcb->b = numero;
		return OK;
	}
	if ('c' == registro)
	{
		loggear_trace("Valor viejo %d -> Valor nuevo %d", tcb->c, numero);
		tcb->c = numero;
		return OK;
	}
	if ('d' == registro)
	{
		loggear_trace("Valor viejo %d -> Valor nuevo %d", tcb->d, numero);
		tcb->d = numero;
		return OK;
	}
	if ('e' == registro)
	{
		loggear_trace("Valor viejo %d -> Valor nuevo %d", tcb->e, numero);
		tcb->e = numero;
		return OK;
	}

	loggear_warning("No existe el registro %c, byte %x", registro, registro);

	return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
}

resultado_t obtener_valor_del_registro(tcb_t* tcb, char registro,
	int32_t* numero)
{
	loggear_trace("Me preparo para obtener valor de registro");

	if ('a' == registro)
	{
		loggear_trace("Valor del registro %c es %d", registro, tcb->a);
		*numero = tcb->a;
		return OK;
	}
	if ('b' == registro)
	{
		loggear_trace("Valor del registro %c es %d", registro, tcb->b);
		*numero = tcb->b;
		return OK;
	}
	if ('c' == registro)
	{
		loggear_trace("Valor del registro %c es %d", registro, tcb->c);
		*numero = tcb->c;
		return OK;
	}
	if ('d' == registro)
	{
		loggear_trace("Valor del registro %c es %d", registro, tcb->d);
		*numero = tcb->d;
		return OK;
	}
	if ('e' == registro)
	{
		loggear_trace("Valor del registro %c es %d", registro, tcb->e);
		*numero = tcb->e;
		return OK;
	}

	loggear_warning("No existe el registro %c, byte %x", registro, registro);

	return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
}

void actualizar_pc(tcb_t* tcb, direccion nuevo_pc)
{
	loggear_trace("Actualizo valor del pc de PID %d", tcb->pid);
	loggear_trace("Vieja pc %x -> Nueva pc %x", tcb->pc, nuevo_pc);
	tcb->pc = nuevo_pc;
}

void actualizar_registro_a(tcb_t* tcb, int32_t nuevo_valor)
{
	loggear_trace("Actualizo registro A del tcb PID %d", tcb->pid);
	loggear_trace("Viejo valor %d -> Nuevo valor %d", tcb->a, nuevo_valor);
	tcb->a = nuevo_valor;
}

void actualizar_registro_b(tcb_t* tcb, int32_t nuevo_valor)
{
	loggear_trace("Actualizo registro B del tcb PID %d", tcb->pid);
	loggear_trace("Viejo valor %d -> Nuevo valor %d", tcb->b, nuevo_valor);
	tcb->b = nuevo_valor;
}

direccion obtener_base_de_codigo(tcb_t* tcb)
{
	return tcb->base_codigo;
}

int32_t obtener_valor_registro_a(tcb_t* tcb)
{
	loggear_trace("Obtengo el valor del registro A del tcb PID %d", tcb->pid);
	loggear_trace("Valor %d", tcb->a);
	return tcb->a;
}

int32_t obtener_valor_registro_b(tcb_t* tcb)
{
	loggear_trace("Obtengo el valor del registro B del tcb PID %d", tcb->pid);
	loggear_trace("Valor %d", tcb->b);
	return tcb->b;
}

resultado_t mover_cursor_stack(tcb_t* tcb, int32_t cantidad_de_bytes)
{
	loggear_trace("Muevo cursor del stack del tcb PID %d", tcb->pid);
	loggear_trace("Posicion actual %d -- Bytes a mover %d", tcb->cursor_stack, cantidad_de_bytes);
	if (tcb->base_stack > tcb->cursor_stack + cantidad_de_bytes) {
		loggear_warning("La cantidad de bytes a mover sobrepasa la base");
		return EXCEPCION_POR_POSICION_DE_STACK_INVALIDA;
	}

	tcb->cursor_stack = tcb->cursor_stack + cantidad_de_bytes;

	loggear_trace("Base stack %d -- Curso stack %d", tcb->base_stack, tcb->cursor_stack);

	return OK;
}
