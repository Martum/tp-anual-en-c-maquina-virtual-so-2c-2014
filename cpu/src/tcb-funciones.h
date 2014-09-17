/*
 * tcb-funciones.h
 *
 *  Created on: 16/09/2014
 *      Author: utnso
 */

#include "estructuras.h"

#ifndef TCB_FUNCIONES_H_
#define TCB_FUNCIONES_H_

/**
 * @NAME: copiar_numero_en_registro
 * @DESC: Actualiza el valor del registro de la tcb con el numero.
 * 		Devuelve 0 si _todo sale bien. Devuelve -1 si no se le paso un registro que exista.
 */
int copiar_numero_en_registro(tcb_t* tcb, char* registro, int* numero);
int obtener_valor_de_registro(tcb_t* tcb, char* registro);

#endif /* TCB_FUNCIONES_H_ */
