/*
 * logs.c
 *
 *  Created on: 03/11/2014
 *      Author: utnso
 */

#include "logs.h"

FILE *fp;

t_log* logger;

// TODO eliminar logger historial (ya no sirve)
//t_log* logger_historial;

void empezar_loggeo()
{

	fp = fopen("logs_cpu.log", "w");

	truncate("logs_cpu.log", 0);

	logger = log_create("logs_cpu.log", "CPU", true, LOG_LEVEL_TRACE);

//	logger_historial =
//		log_create(
//			"/home/utnso/workspace/tp-2014-2c-hardcodeameun4/cpu/logs/logs_cpu_historial.log",
//			"CPU", false, LOG_LEVEL_TRACE);

	loggear_info("Empiezo el loggeo");
}

void finalizar_loggeo()
{
	loggear_info("Finalizo el loggeo");
	fclose(fp);
	log_destroy(logger);
//	log_destroy(logger_historial);
}

void loggear_trace(const char* format, ...)
{
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_trace(logger, nuevo);
//	log_trace(logger_historial, nuevo);
}

void loggear_debug(const char* format, ...)
{
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_debug(logger, nuevo);
//	log_info(logger_historial, nuevo);
}

void loggear_info(const char* format, ...)
{
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_info(logger, nuevo);
//	log_info(logger_historial, nuevo);
}

void loggear_warning(const char* format, ...)
{
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_warning(logger, nuevo);
//	log_warning(logger_historial, nuevo);
}

void loggear_error(const char* format, ...)
{
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_error(logger, nuevo);
//	log_error(logger_historial, nuevo);
}
