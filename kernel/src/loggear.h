/*
 * loggear.h
 *
 *  Created on: 28/11/2014
 *      Author: martin
 */

#ifndef LOGGEAR_H_
#define LOGGEAR_H_

#include <ansisop-panel/panel/kernel.h>
#include <hu4sockets/tcb.h>

typedef enum { MNEW, MREADY, MEXEC, MBLOCK, MEXIT } colas_para_tcb;

typedef struct tcb_con_cola {
	tcb_t* tcb;
	colas_para_tcb cola;
} tcb_con_cola_t;

void cargar_loggeo();

void loggear_instruccion_protegida();

void loggear_conexion_consola(uint32_t pid);

void loggear_desconexion_consola(uint32_t pid);

void loggear_conexion_cpu(uint32_t id);

void loggear_desconexion_cpu(uint32_t id);

void loggear_estado_de_hilos();

#endif /* LOGGEAR_H_ */
