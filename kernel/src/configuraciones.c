/*
 * configuraciones.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#include "configuraciones.h"
#include <commons/config.h>

t_config* configs_kernel;

void cargar_configuraciones(void)
{
	configs_kernel = config_create("config_kernel.txt");
}

uint32_t tamanio_stack()
{
	return config_get_int_value(configs_kernel, "TAMANIO_STACK");
}
