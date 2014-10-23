/*
 * tcb-funciones.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include <stdlib.h>
#include "hu4sockets/tcb.h"

#ifndef TCB_FUNCIONES_H_
#define TCB_FUNCIONES_H_

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
 * @DESC: Mueve el cursor del stack en cantidad de bytes.
 * @RETURNS:
 * 		OK: pudo completar la operacion
 * 		EXCEPCION_POR_LECTURA_DE_STACK_INVALIDA: si cursor - cantidad es menor que base de stack
 */
resultado_t mover_cursor_stack(tcb_t* tcb, int32_t cantidad_de_bytes);

/**
 * @DESC: Obtiene la direccion de la base de codigo de la tcb.
 * @RETURNS: direccion de base de codigo
 */
direccion obtener_base_de_codigo(tcb_t* tcb);

#endif /* TCB_FUNCIONES_H_ */
