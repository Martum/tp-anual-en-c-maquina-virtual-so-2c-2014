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
	inicializar_panel(CPU, "");
}

void _cargar_tcb_actual(t_hilo* tcb_actual, const tcb_t* tcb)
{
	tcb_actual->pid = tcb->pid;
	tcb_actual->tid = tcb->tid;
	tcb_actual->kernel_mode = tcb->km;
}

void _cargar_registros(t_registros_cpu* registros, const tcb_t* tcb)
{
	registros->registros_programacion[0] = tcb->a;
	registros->registros_programacion[1] = tcb->b;
	registros->registros_programacion[2] = tcb->c;
	registros->registros_programacion[3] = tcb->d;
	registros->registros_programacion[4] = tcb->e;
	registros->K = tcb->km;
	registros->X = tcb->base_stack;
	registros->S = tcb->cursor_stack;
	registros->M = tcb->base_codigo;
	registros->P = tcb->tamanio_codigo;
	registros->I = tcb->pid;
}

void ansisop_comienzo_tcb(tcb_t tcb, int32_t quantum)
{
	_cargar_tcb_actual(&tcb_actual, &tcb);
	_cargar_registros(&registros, &tcb);
	comienzo_ejecucion(&tcb_actual, quantum);
}

void ansisop_fin_tcb()
{
	fin_ejecucion();
}

void _agregar_registro_a_parametros(t_list* parametros, char param_registro)
{
	char* registro = malloc(sizeof(char) + 1);
	*(registro) = param_registro;
	*(registro + 1) = '\0';
	list_add(parametros, registro);
}

void _agregar_numero_a_parametros(t_list* parametros, int32_t param_numero)
{
	char* numero = string_itoa(param_numero);
	list_add(parametros, numero);
}

void _free_parametro(void* elemento)
{
	free(elemento);
}

void ansisop_ejecucion_instruccion1(instruccion_t instruccion)
{
	t_list* parametros = list_create();

	ejecucion_instruccion(instruccion, parametros);
}

void ansisop_ejecucion_instruccion2(instruccion_t instruccion,
	int32_t param_numero, char param_registro)
{
	t_list* parametros = list_create();
	_agregar_numero_a_parametros(parametros, param_numero);
	_agregar_registro_a_parametros(parametros, param_registro);

	ejecucion_instruccion(instruccion, parametros);

	list_clean_and_destroy_elements(parametros, _free_parametro);
}

void ansisop_ejecucion_instruccion3(instruccion_t instruccion,
	int32_t param_numero)
{
	t_list* parametros = list_create();
	_agregar_numero_a_parametros(parametros, param_numero);

	ejecucion_instruccion(instruccion, parametros);

	list_clean_and_destroy_elements(parametros, _free_parametro);
}

void ansisop_ejecucion_instruccion4(instruccion_t instruccion,
	int32_t param_numero, char param_registro1, char param_registro2)
{
	t_list* parametros = list_create();
	_agregar_numero_a_parametros(parametros, param_numero);
	_agregar_registro_a_parametros(parametros, param_registro1);
	_agregar_registro_a_parametros(parametros, param_registro2);

	ejecucion_instruccion(instruccion, parametros);

	list_clean_and_destroy_elements(parametros, _free_parametro);
}

void ansisop_ejecucion_instruccion5(instruccion_t instruccion,
	char param_registro)
{
	t_list* parametros = list_create();
	_agregar_registro_a_parametros(parametros, param_registro);

	ejecucion_instruccion(instruccion, parametros);

	list_clean_and_destroy_elements(parametros, _free_parametro);
}

void ansisop_ejecucion_instruccion6(instruccion_t instruccion,
	char param_registro1, char param_registro2)
{
	t_list* parametros = list_create();
	_agregar_registro_a_parametros(parametros, param_registro1);
	_agregar_registro_a_parametros(parametros, param_registro2);

	ejecucion_instruccion(instruccion, parametros);

	list_clean_and_destroy_elements(parametros, _free_parametro);
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

void ansisop_cambio_registro_m(direccion nuevo_valor)
{
	registros.M = nuevo_valor;
	cambio_registros(registros);
}

void ansisop_cambio_registro_pc(direccion nuevo_valor)
{
	registros.P = nuevo_valor;
	cambio_registros(registros);
}

void ansisop_cambio_registro_x(direccion nuevo_valor)
{
	registros.X = nuevo_valor;
	cambio_registros(registros);
}

void ansisop_cambio_registro_cursor(direccion nuevo_valor)
{
	registros.S = nuevo_valor;
	cambio_registros(registros);
}

void ansisop_cambio_registro_k(bool nuevo_valor)
{
	registros.K = nuevo_valor;
	cambio_registros(registros);
}

void ansisop_cambio_registro_i(uint32_t nuevo_valor)
{
	registros.P = nuevo_valor;
	cambio_registros(registros);
}
