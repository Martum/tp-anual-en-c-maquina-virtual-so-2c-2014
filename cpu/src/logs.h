/*
 * logs.h
 *
 *  Created on: 03/11/2014
 *      Author: utnso
 */

#ifndef LOGS_H_
#define LOGS_H_

#include "commons/log.h"

void empezar_loggeo();

void finalizar_loggeo();

void loggear_trace(char* mensaje);
void loggear_info(char* mensaje);
void loggear_warning(char* mensaje);
void loggear_error(char* mensaje);

#endif /* LOGS_H_ */
