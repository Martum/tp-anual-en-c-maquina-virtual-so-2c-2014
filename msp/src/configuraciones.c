/*
 * configuraciones.c
 *
 *  Created on: 22/09/2014
 *      Author: utnso
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "marco.h"
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/string.h>

t_config* configs_msp;
uint32_t memoria_fisica_total;
t_list* lista_procesos;
t_list* lista_marcos;

void cargar_configuraciones(){
	configs_msp = config_create("config_msp.txt");
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
	lista_procesos = list_create();
}

void inicializar_lista_marcos(){
	lista_marcos = list_create();
	uint32_t cant_marcos = cantidad_memoria();
	int i;
	for(i=0;i<cant_marcos;i++){
		// pongo arbitrariamente 10000 como base de marcos, se puede cambiar
		crear_marco_en_lista_de_marcos(lista_marcos,10000);
	}
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


void inicializar_memoria_fisica_total(){
	memoria_fisica_total = cantidad_memoria();
}

// POR AHORA ESTO NO SE NECESITA
/*
char* arma_direccion(uint16_t numero, int cant_bits)
{
		char* aux=malloc(13);
		char* binario =malloc(13);
		bool fin=false;

		while(!fin)
		{
			aux=string_itoa(numero%2);
			string_append(&aux,binario);
			binario=aux;
			numero=numero/2;
			if(numero==0)fin=true;

		}

		int len = strlen(binario);

		while(len<cant_bits)
		{
			aux=string_itoa(0);
			string_append(&aux,binario);
			binario=aux;
			len = strlen(binario);
		}

		return binario;
}
*/
