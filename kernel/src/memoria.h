/*
 * memoria.h
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#ifndef MEMORIA22_H_
#define MEMORIA22_H_

#include <hu4sockets/tcb.h>

/**
 * Conecta con la memoria.
 * @RETURNS: 0 en caso de exito; -1 en caso de fallar
 */
int32_t conectar_con_memoria();

/**
 * @NAME: crear_segmento
 * @DESC: Solicita la creacion de un segmento a la MSP.
 * 		Deja en direccion la direccion del segmento
 * @RETURNS: Devuelve 0 en caso de exito; -1 en caso de Error de Memoria Llena.
 */
int32_t crear_segmento(uint32_t pid, uint32_t tamanio, direccion* direccion);

/**
 * @NAME: crear_segmento
 * @DESC: Solicita la creacion de un segmento a la MSP.
 * 		Deja en direccion la direccion del segmento
 * @RETURNS: Devuelve la respuesta serializada que reteorna la msp ante la petición.
 */
char* crear_segmento_retornando_rta_serializada(uint32_t pid, uint32_t tamanio, direccion* direccion);


/**
 * Destruye el segmento correspondiente
 */
int32_t destruir_segmento(uint32_t pid, direccion base_segmento);

/**
 * Destruye el segmento correspondiente. Retorna la rta serializada que responde la msp.
 */
char* destruir_segmento_retornando_rta_serializada(uint32_t pid, direccion base_segmento);


/**
 * Lee memoria
 */
resultado_t leer_de_memoria(direccion pid, direccion direccion, uint32_t cantidad_de_bytes, char* buffer);

/**
 * Escribe en la memoria los bytes enviados
 *
 * @RETURNS: 0 en caso de exito; -1 si existe Segmentatio Fault
 */
int32_t escribir_memoria(uint32_t pid, direccion direccion, char* bytes_a_escribir, uint32_t cantidad_de_bytes);


#endif /* MEMORIA22_H_ */
