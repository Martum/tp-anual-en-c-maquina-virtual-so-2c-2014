/*
 * consola.h
 *
 *  Created on: 25/9/2014
 *      Author: matias
 */

#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdint.h>

/**
 * recibe por parámetro el identificador de tipo y retorna un puntero a lo que leyó por consola.
 */
//void* leer(uint16_t identificador_tipo);

/*
 * pide leer numeros por consola.
 */
void pedido_leer_int(uint32_t pid);

/*
 * pide leer cadena por consola.
 */
void pedido_leer_cadena(uint32_t pid);

#endif /* CONSOLA_H_ */
