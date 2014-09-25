/*
 * serviciosacpu.h
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#ifndef SERVICIOSACPU_H_
#define SERVICIOSACPU_H_

#include "estructuras.h"

/**
 * BORRAR?? creo que habiamos hecho este primero, despues el que devuelve int y nos olvidmaos de sacarlo.
 * Crea un TCB hilo en base al recibido y lo encola en RDY
 */
void crear_hilo(tcb_t* tcb, uint32_t tid);

/**
 * Crea un nuevo Hilo a partir de TCB y lo asigna a la cola de RDY.
 * Devuelve 0 en caso de exito o -1 en caso de no haber memoria disponible.
 */
int crear_hilo(tcb_t* tcb, uint32_t tid);


#endif /* SERVICIOSACPU_H_ */
