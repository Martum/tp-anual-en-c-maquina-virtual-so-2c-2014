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
