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
 * @DESC: Crea un espacio de memoria para la tcb
 * @RETURNS: el puntero al espacio de memoria creado
 */
tcb_t* crear_tcb();

/*
 * @DESC: Libera el espacio de memoria de la tcb
 * @RETURNS: nada
 */
void liberar_tcb(tcb_t* tcb);

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
void actualizar_pc(tcb_t* tcb, direccion direccion);

/**
 * @DESC: Actualiza el valor del cursor del stack en cantidad de bytes.
 * @RETURNS:
 */
void actualizar_cursor_stack(tcb_t* tcb, int32_t cantidad_de_bytes);

/**
 * @DESC: Obtiene el valor del pc de la tcb.
 * @RETURNS:
 */
void obtener_pc(tcb_t* tcb, direccion* pc);

/**
 * @DESC: Obtiene la direccion de la base de codigo de la tcb.
 * @RETURNS:
 */
void obtener_base_de_codigo(tcb_t* tcb, direccion* base);

#endif /* TCB_FUNCIONES_H_ */
