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
