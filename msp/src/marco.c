/*
 * marco.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */


#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "configuraciones.h"
#include "estructuras.h"
#include "semaforos.h"
#include "funciones_streams.h"

#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>

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
	//lock_lista_marcos();
	marco_t* marco = list_find(get_lista_marcos(), (void*) _is_marco);
	//unlock_lista_marcos();
	return marco;
}

marco_t* buscar_marco_libre()
{
	bool _is_marco_libre(marco_t *m) {
		return m->ocupado == false;
	}
	//lock_lista_marcos();
	marco_t* marco = list_find(get_lista_marcos(), (void*) _is_marco_libre);
	//unlock_lista_marcos();
	return marco;
}

uint32_t cantidad_marcos_libre(){
	uint32_t cant_marcos = 0;

	void _marco_libre(marco_t *m) {
		if(!m->ocupado){
			cant_marcos++;
		}
	}
	//lock_lista_marcos();
	list_iterate(get_lista_marcos(), (void*) _marco_libre);
	//unlock_lista_marcos();
	return cant_marcos;
}

uint32_t _calcular_tamanio_real(uint32_t * tamanio, uint16_t desplazamiento, int * mas_paginas)
{
	//Calculo el tamanio real
		uint32_t tamanio_aux;
		tamanio_aux= *tamanio;
		int aux = (256-desplazamiento-*(tamanio));
		if(aux<0)
			{
				tamanio_aux= 256-desplazamiento;
				*mas_paginas=1;
			}
			else
			{
				*(mas_paginas)=0;
			}

		*(tamanio)=*(tamanio)-tamanio_aux;

		return tamanio_aux;
}



char* leer_marco(char* datos_marco, uint16_t desplazamiento, uint32_t * tamanio, int * mas_paginas)
{

	uint32_t tamano_aux = _calcular_tamanio_real(tamanio, desplazamiento, mas_paginas);

	char* aux = substream(datos_marco,desplazamiento,tamano_aux);
	return aux;
}

void escribir_marco(marco_t* * marco, uint16_t desplazamiento, uint32_t * tamanio, char* * bytes_a_escribir, int * mas_paginas)
{

	uint32_t tamanio_aux = _calcular_tamanio_real(tamanio, desplazamiento, mas_paginas);

	int i;
	for(i=0;i<tamanio_aux;i++)
	{
		char caracter = *((*bytes_a_escribir)+i);
		if(caracter=='\0')
		{
			caracter = ' ';
		}

		(*marco)->datos[i+desplazamiento]=caracter;

	}


}


