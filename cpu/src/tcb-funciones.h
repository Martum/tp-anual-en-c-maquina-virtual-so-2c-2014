/*
 * tcb-funciones.h
 *
 *  Created on: 16/09/2014
 *      Author: utnso
 */

#include <stdint.h>
#include "estructuras.h"

#ifndef TCB_FUNCIONES_H_
#define TCB_FUNCIONES_H_

tcb_t* crear_tcb();

/**
 * @NAME: copiar_valor_en_registro
 * @DESC: Actualiza el valor del registro de la tcb con el numero.
 * 		Devuelve 0 si _todo sale bien. Devuelve -1 si no se le paso un registro que exista.
 */
int32_t actualizar_valor_en_registro(tcb_t* tcb, char registro, int32_t numero);
/**
 * @NAME: obtener_valor_de_registro
 * @DESC: Obtiene el valor del registro de la tcb.
 * 		Devuelve el valor si _todo sale bien. Devuelve -1 si no se le paso un registro que exista.
 */
int32_t obtener_valor_de_registro(tcb_t* tcb, char registro);
/**
 * @NAME: actualizar_pc
 * @DESC: Actualiza el valor del pc con el valor.
 * 		Devuelve 0 si _todo sale bien.
 */
int32_t actualizar_pc(tcb_t* tcb, int32_t valor);
/**
 * @NAME: obtener_pc
 * @DESC: Obtiene el valor del pc de la tcb.
 * 		Devuelve el pc
 */
int32_t obtener_pc(tcb_t* tcb);
/**
 * @NAME: obtener_base_de_codigo
 * @DESC: Obtiene la direccion de la basel de codigo de la tcb.
 * 		Devuelve la direccion de la base de codigo
 */
direccion obtener_base_de_codigo(tcb_t* tcb);

#endif /* TCB_FUNCIONES_H_ */
