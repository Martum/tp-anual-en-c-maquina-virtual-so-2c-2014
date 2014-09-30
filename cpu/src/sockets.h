/*
 * memoria.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
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

/*
 * @DESC: Le pide al kernel un tcb y guarda el tcb devuelto y su quantum y los parametros
 * @RETURNS:
 * 		OK si pudo conectarse y trajo bien los datos
 * 		FALLO_PEDIDO_DE_TCB si hubo problemas al traer los datos
 */
resultado_t pedir_tcb(tcb_t* tcb, int32_t* quantum);

/*
 * @DESC: Le pide a memoria que cree un nuevo segmento
 * @RETURNS:
 * 		OK si pudo conectarse y se creo el nuevo segemento
 * 		FALLO_CREACION_DE_SEGMENTO si hubo problemas al crear el segmento
 */
resultado_t crear_segmento(tcb_t* tcb, uint32_t bytes, direccion* direccion);

/*
 * @DESC: Le pide a memoria que destruya un segmento
 * @RETURNS:
 * 		OK si pudo conectarse y se destruyo el segmento
 * 		FALLO_CONEXION si hubo problemas al conectarse
 * 		todo si hubo problemas al destruir el segmento
 */
int32_t destruir_segmento(tcb_t* tcb, direccion direccion);

/*
 * @DESC: Le pide a memoria que le mande un chorro de datos desde una direccion y el resultado lo guarda en buffer
 * @RETURNS:
 * 		OK si pudo conectarse y trajo bien los datos
 * 		FALLO_CONEXION si hubo problemas al conectarse
 * 		todo si hubo problemas al traer los datos
 */
int32_t leer_de_memoria(tcb_t* tcb, direccion direccion, uint32_t bytes, void* buffer);

/*
 * @DESC: Le pide a memoria que guarde a partir de una direccion un chorro de datos
 * @RETURNS:
 * 		OK si pudo conectarse y escribir los datos
 * 		FALLO_CONEXION si hubo problemas al conectarse
 * 		todo si hubo problemas al escribir los datos
 */
int32_t escribir_en_memoria(tcb_t* tcb, direccion direccion, uint32_t bytes, void* buffer);

/*
 * @DESC: Le avisa al kernel que termino de ejecutar el quantum siendo res el motivo de finalizacion
 * @RETURNS:
 * 		OK si pudo conectarse y mandar bien el mensaje
 * 		FALLO_CONEXION si hubo problemas al conectarse
 * 		todo si no pudo mandar bien el mensaje
 */
int32_t informar_a_kernel_de_finalizacion(tcb_t* tcb, resultado_t res);

/*
 * @DESC: Cierra los puertos de memoria y kernel
 * @RETURNS:
 */
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
