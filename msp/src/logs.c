/*
 * logs.c
 *
 *  Created on: 25/11/2014
 *      Author: utnso
 */

#include "logs.h"
#include <stdbool.h>
#include <unistd.h>
#include "commons/log.h"
#include "commons/string.h"

FILE *fp;
t_log* logger;

void empezar_loggeo(){
	fp = fopen("../logs_msp.log", "w");
	truncate("../logs_msp.log", 0);
	logger = log_create("../logs_msp.log", "MSP", false, LOG_LEVEL_TRACE);
	loggear_info("Empiezo el loggeo");
}

void finalizar_loggeo(){
	loggear_info("Finalizo el loggeo");
	fclose(fp);
	log_destroy(logger);
}

void loggear_trace(const char* format, ...){
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_trace(logger, nuevo);
}

void loggear_debug(const char* format, ...){
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_debug(logger, nuevo);
}

void loggear_info(const char* format, ...){
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_info(logger, nuevo);
}

void loggear_warning(const char* format, ...){
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_warning(logger, nuevo);
}

void loggear_error(const char* format, ...){
	char* nuevo;
	va_list arguments;
	va_start(arguments, format);
	nuevo = string_from_vformat(format, arguments);
	va_end(arguments);
	log_error(logger, nuevo);
}
