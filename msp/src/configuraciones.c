/*
 * configuraciones.c
 *
 *  Created on: 22/09/2014
 *      Author: utnso
 */

#include <stdint.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/config.h>

t_config* configs_msp;
t_list* lista_procesos;
uint32_t memoria_fisica_total;

t_list* get_lista_procesos(){
	return lista_procesos;
}
uint32_t get_memoria_fisica_total(){
	return memoria_fisica_total;
}

void inicializar_lista_procesos(){
	lista_procesos = list_create();
}

void inicializar_memoria_fisica_total(uint32_t tamanio){
	memoria_fisica_total = tamanio;
}
