/*
 * configuraciones.c
 *
 *  Created on: 22/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "marco.h"
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>

t_config* configs_msp;
uint32_t memoria_fisica_total;
t_list* lista_procesos;
t_list* lista_marcos;

// indice de paginas para acceder a todas las paginas rapidamente
t_list* indice_paginas;

void cargar_configuraciones(){
	configs_msp = config_create("/home/utnso/workspace/tp-2014-2c-hardcodeameun4/msp/configs_msp.cfg");
}

void destruir_configuraciones(){
	config_destroy(configs_msp);
}

// /utnso/workspace/tp-2014-2c-hardcodeameun4/msp/
uint32_t puerto(){
	return config_get_int_value(configs_msp, "PUERTO");
}

uint32_t cantidad_memoria(){
	// cambiar
	return config_get_int_value(configs_msp, "CANTIDAD_MEMORIA");
    //return 20;
}

uint32_t cantidad_swap(){
	// cambiar
	return config_get_int_value(configs_msp, "CANTIDAD_SWAP");
//	return 5;
}

char* algoritmo_sustitucion_de_paginas(){
	return config_get_string_value(configs_msp, "SUST_PAGS");
}

void inicializar_lista_procesos(){
	lista_procesos = list_create();
}

void inicializar_lista_marcos(){
	lista_marcos = list_create();
	uint32_t cant_marcos = cantidad_memoria();
	int i = 0;

	for(i=0;i<cant_marcos;i++){

		// pongo arbitrariamente 10000 como base de marcos, se puede cambiar
		crear_marco_en_lista_de_marcos(lista_marcos,2000);


	}
}

void inicializar_memoria_fisica_total(){
	memoria_fisica_total = cantidad_memoria();
}

void inicializar_indice_paginas(){
	indice_paginas = list_create();
}

t_list* get_lista_procesos(){
	return lista_procesos;
}

t_list* get_lista_marcos(){
	return lista_marcos;
}

uint32_t get_memoria_fisica_total(){
	return memoria_fisica_total;
}

t_list* get_indice_paginas(){
	return indice_paginas;
}


