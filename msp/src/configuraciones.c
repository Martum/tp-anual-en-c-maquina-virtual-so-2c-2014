/*
 * configuraciones.c
 *
 *  Created on: 22/09/2014
 *      Author: utnso
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/string.h>

t_config* configs_msp;
t_list* lista_procesos;
uint32_t memoria_fisica_total;

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
