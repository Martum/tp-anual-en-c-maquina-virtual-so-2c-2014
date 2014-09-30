/*
 * tcb-funciones.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

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
resultado_t actualizar_valor_en_registro(tcb_t* tcb, char registro,
		int32_t numero);
/*
 * @DESC: Obtiene el valor del registro de la tcb.
 * @RETURNS:
 * 		OK si _todo sale bien
 * 		NO_ENCONTRO_EL_REGISTRO si se le paso un registro que no exista.
 */
resultado_t obtener_valor_de_registro(tcb_t* tcb, char registro,
		int32_t* numero);

/**
 * @DESC: Actualiza el valor del pc con el valor.
 * @RETURNS:
 */
int32_t actualizar_pc(tcb_t* tcb, int32_t valor);

/**
 * @DESC: Obtiene el valor del pc de la tcb.
 * @RETURNS: la direccion del pc
 */
int32_t obtener_pc(tcb_t* tcb);

/**
 * @DESC: Obtiene la direccion de la base de codigo de la tcb.
 * @RETURNS: la direccion de la base de codigo
 */
direccion obtener_base_de_codigo(tcb_t* tcb);

#endif /* TCB_FUNCIONES_H_ */
