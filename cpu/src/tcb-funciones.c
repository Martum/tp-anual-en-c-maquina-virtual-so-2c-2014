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

// TODO cambiar los ansisop_cambio_registro_e(numero)
resultado_t actualizar_valor_del_registro(tcb_t* tcb, char registro,
	int32_t numero)
{
	if ('A' == registro)
	{
		loggear_debug("Actualizo el valor del registro A, %d -> %d", tcb->a,
			numero);
		ansisop_cambio_registro_a(numero);
		tcb->a = numero;
		return OK;
	}
	if ('B' == registro)
	{
		loggear_debug("Actualizo el valor del registro B, %d -> %d", tcb->b,
			numero);
		ansisop_cambio_registro_b(numero);
		tcb->b = numero;
		return OK;
	}
	if ('C' == registro)
	{
		loggear_debug("Actualizo el valor del registro C, %d -> %d", tcb->c,
			numero);
		ansisop_cambio_registro_c(numero);
		tcb->c = numero;
		return OK;
	}
	if ('D' == registro)
	{
		loggear_debug("Actualizo el valor del registro D, %d -> %d", tcb->d,
			numero);
		ansisop_cambio_registro_d(numero);
		tcb->d = numero;
		return OK;
	}
	if ('E' == registro)
	{
		loggear_debug("Actualizo el valor del registro E, %d -> %d", tcb->e,
			numero);
		ansisop_cambio_registro_e(numero);
		tcb->e = numero;
		return OK;
	}
	if ('M' == registro)
	{
		loggear_debug("Actualizo el valor del registro M, %d -> %d",
			tcb->base_codigo, numero);
		ansisop_cambio_registro_m(numero);
		tcb->base_codigo = numero;
		return OK;
	}
	if ('P' == registro)
	{
		loggear_debug("Actualizo el valor del registro P, %d -> %d", tcb->pc,
			numero);
		ansisop_cambio_registro_pc(numero);
		tcb->pc = numero;
		return OK;
	}
	if ('X' == registro)
	{
		loggear_debug("Actualizo el valor del registro X, %d -> %d",
			tcb->base_stack, numero);
		ansisop_cambio_registro_x(numero);
		tcb->base_stack = numero;
		return OK;
	}
	if ('S' == registro)
	{
		loggear_debug("Actualizo el valor del registro S, %d -> %d",
			tcb->cursor_stack, numero);
		ansisop_cambio_registro_cursor(numero);
		tcb->cursor_stack = numero;
		return OK;
	}
	if ('K' == registro)
	{
		loggear_debug("Actualizo el valor del registro K, %d -> %d", tcb->km,
			numero);
		ansisop_cambio_registro_k(numero);
		tcb->km = numero;
		return OK;
	}
	if ('I' == registro)
	{
		loggear_debug("Actualizo el valor del registro I, %d -> %d", tcb->pid,
			numero);
		ansisop_cambio_registro_i(numero);
		tcb->pid = numero;
		return OK;
	}

	loggear_warning("No existe el registro %c, byte %x", registro, registro);

	return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
}

resultado_t obtener_valor_del_registro(tcb_t* tcb, char registro,
	int32_t* numero)
{
	if ('A' == registro)
	{
		loggear_debug("Obtengo valor del registro A es %d", tcb->a);
		*numero = tcb->a;
		return OK;
	}
	if ('B' == registro)
	{
		loggear_debug("Obtengo valor del registro B es %d", tcb->b);
		*numero = tcb->b;
		return OK;
	}
	if ('C' == registro)
	{
		loggear_debug("Obtengo valor del registro C es %d", tcb->c);
		*numero = tcb->c;
		return OK;
	}
	if ('D' == registro)
	{
		loggear_debug("Obtengo valor del registro D es %d", tcb->d);
		*numero = tcb->d;
		return OK;
	}
	if ('E' == registro)
	{
		loggear_debug("Obtengo valor del registro E es %d", tcb->e);
		*numero = tcb->e;
		return OK;
	}
	if ('M' == registro)
	{
		loggear_debug("Obtengo valor del registro M es %d", tcb->base_codigo);
		*numero = tcb->base_codigo;
		return OK;
	}
	if ('P' == registro)
	{
		loggear_debug("Obtengo valor del registro P es %d", tcb->pc);
		*numero = tcb->pc;
		return OK;
	}
	if ('X' == registro)
	{
		loggear_debug("Obtengo valor del registro X es %d", tcb->base_stack);
		*numero = tcb->base_stack;
		return OK;
	}
	if ('S' == registro)
	{
		loggear_debug("Obtengo valor del registro S es %d", tcb->cursor_stack);
		*numero = tcb->cursor_stack;
		return OK;
	}
	if ('K' == registro)
	{
		loggear_debug("Obtengo valor del registro K es %d", tcb->km);
		*numero = tcb->km;
		return OK;
	}
	if ('I' == registro)
	{
		loggear_debug("Obtengo valor del registro I es %d", tcb->pid);
		*numero = tcb->pid;
		return OK;
	}

	loggear_warning("No existe el registro %c, byte %x", registro, registro);

	return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
}

void actualizar_pc(tcb_t* tcb, direccion nuevo_pc)
{
	loggear_debug("Actualizo el valor del registro P, %d -> %d", tcb->pc,
		nuevo_pc);
	ansisop_cambio_registro_pc(nuevo_pc);
	tcb->pc = nuevo_pc;
}

void actualizar_registro_a(tcb_t* tcb, int32_t nuevo_valor)
{
	loggear_debug("Actualizo el valor del registro A, %d -> %d", tcb->a,
		nuevo_valor);
	ansisop_cambio_registro_a(nuevo_valor);
	tcb->a = nuevo_valor;
}

void actualizar_registro_b(tcb_t* tcb, int32_t nuevo_valor)
{
	loggear_debug("Actualizo el valor del registro B, %d -> %d", tcb->b,
		nuevo_valor);
	ansisop_cambio_registro_b(nuevo_valor);
	tcb->b = nuevo_valor;
}

direccion obtener_base_de_codigo(tcb_t* tcb)
{
	return tcb->base_codigo;
}

int32_t obtener_valor_registro_a(tcb_t* tcb)
{
	loggear_debug("Obtengo valor del registro A es %d", tcb->a);
	return tcb->a;
}

int32_t obtener_valor_registro_b(tcb_t* tcb)
{
	loggear_debug("Obtengo valor del registro B es %d", tcb->b);
	return tcb->b;
}

resultado_t mover_cursor_stack(tcb_t* tcb, int32_t cantidad_de_bytes)
{
	loggear_debug("Muevo cursor del stack, %x -> %x", tcb->cursor_stack,
		tcb->cursor_stack + cantidad_de_bytes);

	if (tcb->base_stack > tcb->cursor_stack + cantidad_de_bytes)
	{
		loggear_warning("La cantidad de bytes a mover sobrepasa la base");
		return EXCEPCION_POR_POSICION_DE_STACK_INVALIDA;
	}

	tcb->cursor_stack = tcb->cursor_stack + cantidad_de_bytes;

	ansisop_cambio_registro_cursor(tcb->cursor_stack);
	loggear_debug("Base stack %x -- Curso stack %x", tcb->base_stack,
		tcb->cursor_stack);

	return OK;
}
