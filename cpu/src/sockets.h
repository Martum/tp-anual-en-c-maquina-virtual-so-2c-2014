/*
 * memoria.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hu4sockets/sockets.h"
#include "hu4sockets/mensajes.h"
#include "instrucciones-estructuras.h"


#ifndef MEMORIA_H_
#define MEMORIA_H_

/*
 * @DESC: Crea el socket sock_t y se conecta con el ip y puerto
 * 			que levanta del archivo de configuracion
 * @RETURNS:
 * 		OK si pudo conectarse
 * 		FALLO_CONEXION si hubo problemas al conectarse
 */
resultado_t conectar_con_memoria();

/*
 * @DESC: Crea el socket sock_t y se conecta con el ip y puerto
 * 			que levanta del archivo de configuracion
 * @RETURNS:
 * 		OK si pudo conectarse
 * 		FALLO_CONEXION si hubo problemas al conectarse
 */
resultado_t conectar_con_kernel();
resultado_t pedir_tcb(tcb_t* tcb, int32_t* quantum);
direccion crear_segmento(tcb_t* tcb, uint32_t bytes);
int32_t destruir_segmento(tcb_t* tcb, direccion direccion);
int32_t leer_de_memoria(tcb_t* tcb, direccion direccion, uint32_t bytes, void* buffer);
int32_t escribir_en_memoria(tcb_t* tcb, direccion direccion, uint32_t bytes, void* buffer);
int32_t informar_a_kernel_de_finalizacion(tcb_t* tcb, resultado_t res);
void cerrar_puertos();

/*
 * @DESC: Lee de memoria los primeros 4 bytes apuntados por pc y actualiza instruccion.
 * @RETURNS:
 */
void obtener_instruccion(tcb_t* tcb, instruccion_t instruccion);

/*
 * @DESC: Lee de memoria un byte apuntado por pc.
 * @RETURNS: el char leido
 */
char obtener_registro(tcb_t* tcb);

/*
 * @DESC: Lee de memoria los primeros 4 bytes apuntados por pc.
 * @RETURNS: el numero leido
 */
int32_t obtener_numero(tcb_t* tcb);

#endif /* MEMORIA_H_ */
