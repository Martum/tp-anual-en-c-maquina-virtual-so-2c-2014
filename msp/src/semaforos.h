/*
 * semaforos.h
 *
 *  Created on: 30/10/2014
 *      Author: utnso
 */

#ifndef SEMAFOROS_H_
#define SEMAFOROS_H_


void lock_lista_procesos();
void unlock_lista_procesos();

void lock_lista_marcos();
void unlock_lista_marcos();

void lock_lista_indice_paginas();
void unlock_lista_indice_paginas();


#endif /* SEMAFOROS_H_ */
