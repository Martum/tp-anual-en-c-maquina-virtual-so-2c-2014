/*
 * logs.h
 *
 *  Created on: 03/11/2014
 *      Author: utnso
 */

#ifndef LOGS_H_
#define LOGS_H_

#include "commons/log.h"
#include "commons/string.h"

void empezar_loggeo();

void finalizar_loggeo();

void loggear_trace(const char* mensaje, ...);
void loggear_info(const char* mensaje, ...);
void loggear_warning(const char* mensaje, ...);
void loggear_error(const char* mensaje, ...);

#endif /* LOGS_H_ */
