/*
 * tcb-funciones.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include <stdlib.h>
#include <string.h>
#include "hu4sockets/tcb.h"

#ifndef TCB_FUNCIONES_H_
#define TCB_FUNCIONES_H_

tcb_t* crear_tcb();
void liberar_tcb();
void clonar_tcb(tcb_t* destino, tcb_t* fuente);
bool es_tcb_kernel(tcb_t* tcb);

/*
 * @DESC: Actualiza el valor del registro de la tcb con el numero.
 * @RETURNS:
 * 		OK si _todo sale bien
 * 		NO_ENCONTRO_EL_REGISTRO si se le paso un registro que no exista.
 */
resultado_t actualizar_valor_del_registro(tcb_t* tcb, char registro,
	int32_t numero);

/*
 * @DESC: Obtiene el valor del registro de la tcb.
 * @RETURNS:
 * 		OK si _todo sale bien
 * 		NO_ENCONTRO_EL_REGISTRO si se le paso un registro que no exista.
 */
resultado_t obtener_valor_del_registro(tcb_t* tcb, char registro,
	int32_t* numero);

/**
 * @DESC: Actualiza el valor del pc con el valor.
 * @RETURNS:
 */
void actualizar_pc(tcb_t* tcb, direccion nuevo_pc);

/**
 * @DESC: Actualiza el valor del tid con el valor.
 * @RETURNS:
 */
void actualizar_tid(tcb_t* tcb, int32_t nuevo_tid);

/**
 * @DESC: Actualiza el valor del km con el nuevo_km.
 * @RETURNS:
 */
void actualizar_km(tcb_t* tcb, bool nuevo_km);

/**
 * @DESC: Actualiza el valor del registro a con nuevo_valor.
 * @RETURNS:
 */
void actualizar_registro_a(tcb_t* tcb, int32_t nuevo_valor);

/**
 * @DESC: Actualiza el valor del registro b con nuevo_valor.
 * @RETURNS:
 */
void actualizar_registro_b(tcb_t* tcb, int32_t nuevo_valor);

/**
 * @DESC: Actualiza la base del stack con nueva_base.
 * @RETURNS:
 */
void actualizar_base_del_stack(tcb_t* tcb, direccion nuevo_base);

/**
 * @DESC: Obtiene la direccion de la base de codigo de la tcb.
 * @RETURNS: direccion de base de codigo
 */
direccion obtener_base_de_codigo(tcb_t* tcb);

/**
 * @DESC: Obtiene el valor del registro a.
 * @RETURNS: valor del registro a
 */
int32_t obtener_valor_registro_a(tcb_t* tcb);

/**
 * @DESC: Obtiene el valor del registro b.
 * @RETURNS: valor del registro b
 */
int32_t obtener_valor_registro_b(tcb_t* tcb);

/**
 * @DESC: Mueve el cursor del stack en cantidad de bytes.
 * @RETURNS:
 * 		OK: pudo completar la operacion
 * 		EXCEPCION_POR_LECTURA_DE_STACK_INVALIDA: si cursor - cantidad es menor que base de stack
 */
resultado_t mover_cursor_stack(tcb_t* tcb, int32_t cantidad_de_bytes);

#endif /* TCB_FUNCIONES_H_ */
