/*
 * configuraciones.c
 *
 *  Created on: 12/10/2014
 *      Author: martin
 */

#include "configuraciones.h"
#include <commons/config.h>

t_config* configs;

void cargar_configuraciones(void)
{
	configs = config_create("config_consola.txt");
}

char* ip_kernel()
{
	return config_get_string_value(configs, "IP_KERNEL");
}

uint32_t puerto_kernel()
{
	return config_get_int_value(configs, "PUERTO_KERNEL");
}
