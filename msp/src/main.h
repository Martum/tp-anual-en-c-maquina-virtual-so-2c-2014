/*
 * main.h
 *
 *  Created on: 16/10/2014
 *      Author: utnso
 */

#ifndef MAIN_H_
#define MAIN_H_

/*
 * Funciones que disparan los hilos: - escuchar consola msp
 * 									 - escuchar cpus
 */
void* escuchar_consola_msp(void*);
void* escuchar_cpus(void*);

#endif /* MAIN_H_ */
