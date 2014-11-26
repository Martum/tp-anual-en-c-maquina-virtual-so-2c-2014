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
#include "semaforos.h"

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

uint32_t cantidad_archivos_swap;

void cargar_configuraciones(){
	configs_msp = config_create("../configs_msp.cfg");
}

void destruir_configuraciones(){
	config_destroy(configs_msp);
}

uint32_t puerto(){
	return config_get_int_value(configs_msp, "PUERTO");
}

uint32_t cantidad_memoria(){
	return config_get_int_value(configs_msp, "CANTIDAD_MEMORIA");
}

uint32_t cantidad_swap(){
	return config_get_int_value(configs_msp, "CANTIDAD_SWAP");
}

char* algoritmo_sustitucion_de_paginas(){
	return config_get_string_value(configs_msp, "SUST_PAGS");
}

void inicializar_lista_procesos(){
	//lock_lista_procesos();
	lista_procesos = list_create();
	//unlock_lista_procesos();
}

void inicializar_lista_marcos(){

//	lock_lista_marcos();
	lista_marcos = list_create();
//	unlock_lista_marcos();

	uint32_t cant_marcos = cantidad_memoria() * 4;
	int i = 0;

	for(i=0;i<cant_marcos;i++){
		//lock_lista_marcos();
		crear_marco_en_lista_de_marcos(lista_marcos);
		//unlock_lista_marcos();
	}
}

void inicializar_memoria_fisica_total(){
	memoria_fisica_total = cantidad_memoria();
}

void inicializar_indice_paginas(){
	//lock_lista_indice_paginas();
	indice_paginas = list_create();
	//unlock_lista_indice_paginas();
}

void inicializar_cantidad_archivos_swap(){
	cantidad_archivos_swap = 0;
}

void aumento_cantidad_archivos_swap(){
	cantidad_archivos_swap++;
}

void disminuyo_cantidad_archivos_swap(){
	cantidad_archivos_swap--;
}

uint32_t get_cantidad_archivos_swap(){
	return cantidad_archivos_swap;
}

t_list* get_lista_procesos(){
	//unlock_lista_procesos();
	t_list* lista = lista_procesos;
	//lock_lista_procesos();
	return lista;
}

t_list* get_lista_marcos(){
	//lock_lista_marcos();
	t_list* lista = lista_marcos;
	//unlock_lista_marcos();
	return lista;
}

uint32_t get_memoria_fisica_total(){
	return memoria_fisica_total;
}

t_list* get_indice_paginas(){
	//lock_lista_indice_paginas();
	t_list* lista = indice_paginas;
	//unlock_lista_indice_paginas();
	return lista;
}


