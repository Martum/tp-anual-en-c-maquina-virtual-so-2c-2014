/*
 * marco.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */
#include <commons/collections/list.h>
#include "configuraciones.h"
#include "estructuras.h"
#include <stdlib.h>
#include <stdbool.h>
#include <commons/log.h>

void crear_marco_en_lista_de_marcos(t_list *marcos, uint32_t base_de_marcos)
{
	marco_t *marco = malloc(sizeof(marco_t));
	//marco->base = list_size(marcos)*256 + base_de_marcos;
	marco->datos= malloc(256);
	marco->id = list_size(marcos);
	marco->ocupado = false;
	list_add(marcos,marco);
}

marco_t* buscar_marco_segun_id(uint32_t id){
	bool _is_marco(marco_t *m) {
		return m->id == id;
	}
	return list_find(get_lista_marcos(), (void*) _is_marco);
}

marco_t* buscar_marco_libre()
{
	bool _is_marco_libre(marco_t *m) {
			return m->ocupado == false;
		}
		return list_find(get_lista_marcos(), (void*) _is_marco_libre);
}


char* leer_marco(char* datos_marco, uint16_t desplazamiento, uint32_t tamanio, bool mas_paginas)
{
	uint32_t tamanio_aux = tamanio;
	if((256-desplazamiento-tamanio)<0)
	{
		tamanio_aux= 256-desplazamiento;
		mas_paginas=true;
	}
	else
	{
		mas_paginas=false;
	}

	tamanio=tamanio-tamanio_aux;
	//Aunque haya o no más paginas, despues de una lectura no va a haber más desplazamiento
	desplazamiento=0;

	return string_substring(datos_marco,desplazamiento,tamanio_aux);
}
