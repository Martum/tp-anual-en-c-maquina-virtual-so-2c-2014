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


/**
 * Recibe un pid y un identificador de tipo y pide a la consola que ingrese un dato segun el identificador.
 * Retorna 0 en caso de exito o -1 en caso de error. (Puede haber error? lo unico que se me ocurre es que se
 * desconecte la consola, pero eso mataria al proyecto directamente).
 */
int entrada_estandar(uint32_t pid, uint16_t identificador_tipo);

/**
 * Recibe un pid y una cadena de texto. Escribe en la consola correspondiente al pid la cadena recibida.
 *  Retorna 0 en caso de exito o -1 en caso de error. (Puede haber error? lo unico que se me ocurre es que se
 * desconecte la consola, pero eso mataria al proyecto directamente).
 */
int salida_estandar(uint32_t pid, char* cadena_a_escribir);

/*
 * Envia el hilo a block, lo encola en la cola de espera del recurso.
 */
int bloquear(tcb_t* tcb, uint32_t recurso);

/*
 * Remueve el primer hilo de la cola de espera del recurso y lo manda a ready.
 */
int despertar(uint32_t recurso);

#endif /* SERVICIOSACPU_H_ */
