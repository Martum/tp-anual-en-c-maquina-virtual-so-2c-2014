/*
 * memoria.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include "estructuras.h"

#ifndef MEMORIA_H_
#define MEMORIA_H_

int conectar_con_memoria();
int conectar_con_kernel();
int pedir_tcb(tcb_t* tcb, int* quantum);
int leer_de_memoria(direccion dir, int bytes, void* buffer);
int informar_a_kernel_de_finalizacion(tcb_t* tcb, int res);
char* obtener_registro(tcb_t* tcb);
int* obtener_numero(tcb_t* tcb);

#endif /* MEMORIA_H_ */
