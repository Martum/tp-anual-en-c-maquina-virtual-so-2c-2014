/*
 * memoria.h
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "estructuras.h"

/**
 * @NAME: crear_segmento
 * @DESC: Solicita la creacion de un segmento a la MSP.
 * 		Devuelve la direccion virtual base del segmento, -1 en caso de Error de Memoria Llena.
 */
direccion crear_segmento(uint32_t pid, uint32_t tamanio);

#endif /* MEMORIA_H_ */
