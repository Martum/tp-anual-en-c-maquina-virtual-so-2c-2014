/*
 * logs.c
 *
 *  Created on: 03/11/2014
 *      Author: utnso
 */

#include "logs.h"

FILE *fp;

t_log* logger;

void empezar_loggeo()
{
	fp = fopen("logs_cpu.log", "w");

	truncate("logs_cpu.log", 0);

	logger = log_create("logs_cpu.log", "CPU", true, LOG_LEVEL_TRACE);

	loggear_info("Empiezo el loggeo");
}

void finalizar_loggeo()
{
	fclose(fp);

	log_destroy(logger);

	loggear_info("Finalizo el loggeo");
}

void loggear_trace(const char* format, ...)
{
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_trace(logger, nuevo);
	free(nuevo);
}

void loggear_debug(const char* format, ...)
{
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_debug(logger, nuevo);
	free(nuevo);
}

void loggear_info(const char* format, ...)
{
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_info(logger, nuevo);
	free(nuevo);
}

void loggear_warning(const char* format, ...)
{
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_warning(logger, nuevo);
	free(nuevo);
}

void loggear_error(const char* format, ...)
{
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_error(logger, nuevo);
	free(nuevo);
}
