/*
 * memoria.h
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <hu4sockets/tcb.h>

/**
 * @NAME: crear_segmento
 * @DESC: Solicita la creacion de un segmento a la MSP.
 * 		Deja en direccion la direccion del segmento
 * @RETURNS: Devuelve 0 en caso de exito; -1 en caso de Error de Memoria Llena.
 */
int32_t crear_segmento(uint32_t pid, uint32_t tamanio, direccion* direccion);

/**
 * Destruye el segmento correspondiente
 */
void destruir_segmento(uint32_t pid, direccion base_segmento);
#endif /* MEMORIA_H_ */
