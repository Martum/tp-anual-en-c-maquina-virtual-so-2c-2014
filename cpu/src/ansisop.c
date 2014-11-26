/*
 * ansisop.c
 *
 *  Created on: 25/11/2014
 *      Author: utnso
 */

#include "ansisop.h"

t_registros_cpu registros;
t_hilo tcb_actual;

void empezar_ansisop()
{
	inicializar_panel(CPU,
		"/home/utnso/workspace/tp-2014-2c-hardcodeameun4/cpu/panel");
}

void ansisop_comienzo_tcb(tcb_t tcb, int32_t quantum)
{
	tcb_actual.pid = tcb.pid;
	tcb_actual.tid = tcb.tid;
	tcb_actual.kernel_mode = tcb.km;
	comienzo_ejecucion(&tcb_actual, quantum);
}

void ansisop_fin_tcb()
{
	fin_ejecucion();
}

void ansisop_ejecucion_instruccion(instruccion_t instruccion,
	int32_t param_numero, t_list* param_registros)
{
	t_list* parametros = list_create();
	char* numero = string_itoa(param_numero);
	list_add(parametros, numero);
	list_add_all(parametros, param_registros);
	ejecucion_instruccion( instruccion, parametros);
}

void _actualizar_registro(int32_t registro, int32_t nuevo_valor)
{
	registros.registros_programacion[registro] = nuevo_valor;
	cambio_registros(registros);
}

void ansisop_cambio_registro_a(int32_t nuevo_valor)
{
	_actualizar_registro(0, nuevo_valor);
}

void ansisop_cambio_registro_b(int32_t nuevo_valor)
{
	_actualizar_registro(1, nuevo_valor);
}

void ansisop_cambio_registro_c(int32_t nuevo_valor)
{
	_actualizar_registro(2, nuevo_valor);
}

void ansisop_cambio_registro_d(int32_t nuevo_valor)
{
	_actualizar_registro(3, nuevo_valor);
}

void ansisop_cambio_registro_e(int32_t nuevo_valor)
{
	_actualizar_registro(4, nuevo_valor);
}

void ansisop_cambio_registro_pc(direccion nuevo_valor)
{
	registros.P = nuevo_valor;
	cambio_registros(registros);
}

void ansisop_cambio_registro_cursor(direccion nuevo_valor)
{
	registros.S = nuevo_valor;
	cambio_registros(registros);
}
