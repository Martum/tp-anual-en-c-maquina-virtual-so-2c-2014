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
#include <commons/string.h>

void crear_marco_en_lista_de_marcos(t_list *marcos)
{
	marco_t *marco = malloc(sizeof(marco_t));
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

uint32_t cantidad_marcos_libre(){
	uint32_t cant_marcos = 0;

	void _marco_libre(marco_t *m) {
		if(!m->ocupado){
			cant_marcos++;
		}
	}

	list_iterate(get_lista_marcos(), (void*) _marco_libre);
	return cant_marcos;
}


char* leer_marco(char* datos_marco, uint16_t desplazamiento, uint32_t tamanio, bool mas_paginas)
{

	uint32_t tamanio_aux = calcular_tamanio_real(tamanio,desplazamiento,mas_paginas);


	return string_substring(datos_marco,desplazamiento,tamanio_aux);
}

void escribir_marco(char* datos_marco, uint16_t desplazamiento, uint32_t tamanio, char* bytes_a_escribir, bool mas_paginas)
{


	uint32_t tamanio_aux = calcular_tamanio_real(tamanio,desplazamiento,mas_paginas);

	int i;
	int j=0;
	for(i=desplazamiento;i<tamanio_aux;i++)
	{
		datos_marco[i]=bytes_a_escribir[j];
		j++;
	}



}

uint32_t calcular_tamanio_real(uint32_t tamanio, uint16_t desplazamiento, bool mas_paginas)
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

		return tamanio_aux;
}


