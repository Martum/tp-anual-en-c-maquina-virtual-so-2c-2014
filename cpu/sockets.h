/*
 * memoria.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

int conectar_con_memoria();
int conectar_con_kernel();
int pido_tcb(tcb_t tcb, int* quantum);

#endif /* MEMORIA_H_ */
