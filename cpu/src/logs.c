/*
 * logs.c
 *
 *  Created on: 03/11/2014
 *      Author: utnso
 */

#include "logs.h"

t_log* logger;
t_log* logger_historial;

void empezar_loggeo()
{
	logger = log_create(
			"/home/utnso/workspace/tp-2014-2c-hardcodeameun4/cpu/logs/logs_cpu.log",
			"CPU", true, LOG_LEVEL_TRACE);
	logger_historial = log_create(
			"/home/utnso/workspace/tp-2014-2c-hardcodeameun4/cpu/logs/logs_cpu_historial.log",
			"CPU", false, LOG_LEVEL_TRACE);
}

void finalizar_loggeo()
{
	log_destroy(logger);
	log_destroy(logger_historial);
}

void loggear_trace(char* mensaje)
{
	log_trace(logger, mensaje);
	log_trace(logger_historial, mensaje);
}

void loggear_info(char* mensaje)
{
	log_info(logger, mensaje);
	log_info(logger_historial, mensaje);
}

void loggear_warning(char* mensaje)
{
	log_warning(logger, mensaje);
	log_warning(logger_historial, mensaje);
}

void loggear_error(char* mensaje)
{
	log_error(logger, mensaje);
	log_error(logger_historial, mensaje);
}
