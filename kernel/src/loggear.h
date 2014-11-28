/*
 * loggear.h
 *
 *  Created on: 28/11/2014
 *      Author: martin
 */

#ifndef LOGGEAR_H_
#define LOGGEAR_H_

#include <ansisop-panel/panel/kernel.h>

void cargar_loggeo();

void loggear_instruccion_protegida();

void loggear_conexion_consola(uint32_t pid);

void loggear_desconexion_consola(uint32_t pid);

void loggear_conexion_cpu(uint32_t id);

void loggear_desconexion_cpu(uint32_t id);

#endif /* LOGGEAR_H_ */
