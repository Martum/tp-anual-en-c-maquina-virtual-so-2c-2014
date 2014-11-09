/*
 * tcb-funciones.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include "logs.h"
#include "tcb-funciones.h"

// TODO eliminar (ya no se necesita)
//tcb_t* crear_tcb()
//{
//	return malloc(sizeof(tcb_t));
//}
//
//void liberar_tcb(tcb_t* tcb)
//{
//	free(tcb);
//}
//
//void clonar_tcb(tcb_t* destino, tcb_t* fuente)
//{
//	memcpy(destino, fuente, sizeof(tcb_t));
//}

bool es_tcb_kernel(tcb_t* tcb)
{
	return tcb->km;
}

// TODO eliminar (ya no se necesita)
//uint32_t obtener_ocupacion_stack(tcb_t* tcb)
//{
//	return tcb->cursor_stack - tcb->base_stack;
//}

resultado_t actualizar_valor_del_registro(tcb_t* tcb, char registro,
	int32_t numero)
{
	loggear_trace("Me preparo para actualizar valor de registro");

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

	loggear_warning("El registro %c no existe", registro);

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

	loggear_warning("No existe el registro %c", registro);

	return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
}

void actualizar_pc(tcb_t* tcb, direccion nuevo_pc)
{
	loggear_trace("Actualizo valor del pc de PID %d", tcb->pid);
	loggear_trace("Vieja pc %d -> Nueva pc %d", tcb->pc, nuevo_pc);
	tcb->pc = nuevo_pc;
}

// TODO eliminar (ya no se necesita)
//void actualizar_tid(tcb_t* tcb, int32_t nuevo_tid)
//{
//	tcb->km = nuevo_tid;
//}

// TODO eliminar (ya no se necesita)
//void actualizar_km(tcb_t* tcb, bool nuevo_km)
//{
//	tcb->km = nuevo_km;
//}

// TODO eliminar (ya no se necesita)
//void actualizar_base_del_stack(tcb_t* tcb, direccion nueva_base)
//{
//	tcb->base_stack = nueva_base;
//}

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
	loggear_trace("Me preparo para mover curso del stack del tcb PID %d", tcb->pid);
	loggear_trace("Posicion actual %d -- Bytes a mover %d", tcb->cursor_stack, cantidad_de_bytes);
	if (tcb->base_stack < tcb->cursor_stack + cantidad_de_bytes) {
		loggear_warning("La cantidad de bytes a mover sobrepasa la base");
		return EXCEPCION_POR_POSICION_DE_STACK_INVALIDA;
	}

	tcb->cursor_stack = tcb->cursor_stack + cantidad_de_bytes;

	loggear_trace("Base stack %d -- Curso stack %d", tcb->base_stack, tcb->cursor_stack);

	return OK;
}
