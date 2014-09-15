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
int pedir_tcb(tcb_t tcb, int* quantum);
int leer_de_memoria(direccion tcb, int bytes, char* buffer);

#endif /* MEMORIA_H_ */
