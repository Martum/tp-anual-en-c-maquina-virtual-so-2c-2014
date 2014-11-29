/*
 * configuraciones.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#include "configuraciones.h"
#include <commons/config.h>

t_config* configs_kernel;
char* path = "config_kernel.cfg";

void cargar_configuraciones(void)
{
	configs_kernel = config_create(path);
}

uint32_t tamanio_stack()
{
	return config_get_int_value(configs_kernel, "TAMANIO_STACK");
}


uint32_t puerto()
{
	return config_get_int_value(configs_kernel, "PUERTO");
}

char* ip_msp()
{
	return config_get_string_value(configs_kernel, "IP_MSP");
}

uint32_t puerto_msp()
{
	return config_get_int_value(configs_kernel, "PUERTO_MSP");
}

uint32_t quantum()
{
	return config_get_int_value(configs_kernel, "QUANTUM");
}

char* syscalls()
{
	return config_get_string_value(configs_kernel, "SYSCALLS");
}

char* ruta_logeo()
{
	return config_get_string_value(configs_kernel, "RUTA_LOGEO");
}
