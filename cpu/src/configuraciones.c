/*
 * configuraciones.c
 *
 *  Created on: 22/09/2014
 *      Author: utnso
 */

#include "configuraciones.h"

t_config* configs_cpu;

resultado_t cargar_configuraciones(){
	configs_cpu = config_create("/home/utnso/workspace/tp-2014-2c-hardcodeameun4/cpu/configs_cpu.cfg");
	if (config_keys_amount(configs_cpu) == 0) {
		return FALLO_CARGA_DE_CONFIGURACIONES;
	}
	return OK;
}

void liberar_configuraciones(){
	config_destroy(configs_cpu);
}

char* ip_kernel(){
	return config_get_string_value(configs_cpu, "IP_KERNEL");
}

char* ip_msp(){
	return config_get_string_value(configs_cpu, "IP_MSP");
}

uint32_t puerto_kernel(){
	return config_get_int_value(configs_cpu, "PUERTO_KERNEL");
}

uint32_t puerto_msp(){
	return config_get_int_value(configs_cpu, "PUERTO_MSP");
}

uint32_t retardo(){
	return config_get_int_value(configs_cpu, "RETARDO");
}
