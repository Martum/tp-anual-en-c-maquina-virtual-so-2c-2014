/*
 * memoria.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdint.h>
#include "estructuras.h"

#ifndef MEMORIA_H_
#define MEMORIA_H_

int32_t conectar_con_memoria();
int32_t conectar_con_kernel();
int32_t pedir_tcb(tcb_t* tcb, int32_t* quantum);
direccion crear_segmento(tcb_t* tcb, int32_t bytes);
int32_t destruir_segmento(tcb_t* tcb, direccion direccion);
int32_t leer_de_memoria(direccion dir, int32_t bytes, void* buffer);
int32_t escribir_en_memoria(direccion dir, int32_t byes, void* buffer);
int32_t informar_a_kernel_de_finalizacion(tcb_t* tcb, int32_t res);
char obtener_registro(tcb_t* tcb);
int32_t obtener_numero(tcb_t* tcb);

#endif /* MEMORIA_H_ */
