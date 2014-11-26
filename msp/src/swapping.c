/*
 * swapping.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "swapping.h"
#include "estructuras.h"
#include "pagina.h"
#include "marco.h"
#include "interfaz.h"
#include "configuraciones.h"
#include "algoritmos_sustitucion.h"
#include "funciones_streams.h"
#include "logs.h"

#include <commons/txt.h>
#include <commons/string.h>


void swap_in(pagina_t* * pagina, uint32_t pid)
{
	marco_t* marco = liberar_un_marco();

	//Modifico la pagina y el marco para que se relacionen entre si
	(*pagina)->marco=marco->id;
	(*pagina)->tiene_marco=true;
	marco->id_proceso = pid;
	marco->ocupado=true;

	loggear_trace("Se asigno el marco %d al proceso %d.", marco->id, pid);
}


marco_t* liberar_un_marco()
{
	uint16_t id_pag_swap;
	pagina_t* pagina_a_liberar;
	segmento_t* segmento_contenedor;
	//SACAR EL NULL
	uint32_t id_marco_a_liberar= realizar_algoritmo_swapping(&id_pag_swap);

	marco_t* marco_a_liberar= buscar_marco_segun_id(id_marco_a_liberar);

	obtener_segmento_y_pagina(&pagina_a_liberar, &segmento_contenedor, id_pag_swap, marco_a_liberar->id_proceso);

	mover_a_disco(&pagina_a_liberar, marco_a_liberar->id_proceso, segmento_contenedor->id);

	return marco_a_liberar;
}

//Debe deolver una pagina*
uint32_t realizar_algoritmo_swapping(uint16_t * id_pagina_swap)
{
	char* algoritmo=algoritmo_sustitucion_de_paginas();
	uint32_t id_marco;

	if(string_equals_ignore_case("CLOCK", algoritmo)){
		id_marco=algoritmo_clock(id_pagina_swap);
	}
	else if(string_equals_ignore_case("LRU", algoritmo)){
		id_marco=algoritmo_lru(id_pagina_swap);
	}

	return id_marco;

}

void obtener_segmento_y_pagina(pagina_t* * pagina_swap,segmento_t* * segmento_contenedor, uint16_t id_pag_swap, uint32_t pid)
{
	/*
	 * TODO Si esta cosa funciona soy un nuevo tipo de Dios
	 *
	 * Dado un segmento checkea si contiene la pagina
	 */
	bool _segmento_contiene_pagina(segmento_t* seg){
			bool _es_pagina(pagina_t* pag)
			{
				return pag->id==id_pag_swap;
			}
			return list_any_satisfy(seg->paginas, (void*)_es_pagina);
		}
	proceso_msp_t* proceso_contenedor = buscar_proceso_segun_pid(pid);
	(*segmento_contenedor)=(segmento_t*)list_find(proceso_contenedor->segmentos,(void*)_segmento_contiene_pagina);//TODO no lo encuentra, creo que porque no esta bien cargado desde interfaz

	(*pagina_swap)=buscar_pagina_segun_id_en_lista_paginas(id_pag_swap, (*segmento_contenedor)->paginas);

}



void mover_a_disco(pagina_t* * pagina, uint32_t pid, uint16_t id_segmento)
{

	//Convierte cada id a string y despues los concatena de 2 en 2
	char *nombre_archivo;
	nombre_archivo=concat_string(string_itoa(pid),string_itoa(id_segmento));
	nombre_archivo=concat_string(nombre_archivo,string_itoa((*pagina)->id));

	char* path;
	path=concat_string("en_disco/",nombre_archivo);
	path=concat_string(path,".txt");
	free(nombre_archivo);
	//crea un archivo y lo guarda en una carpeta interna.
	//el nombre se compone de pid, idsegmento y id pagina
	FILE* arch = txt_open_for_append(path);
	free(path);

	marco_t* marco= buscar_marco_segun_id((*pagina)->marco);
	//Preparo el string que voy a escribir

	char* string_a_escribir=marco->datos;


	//Escribo en el archivo
	txt_write_in_file(arch, string_a_escribir);

	(*pagina)->en_disco=true;

	(*pagina)->tiene_marco=false;

	txt_close_file(arch);

	aumento_cantidad_archivos_swap();
}

void swap_out(uint32_t pid, uint16_t id_segmento, pagina_t* * pagina)
{
	// ----Abro el archivo----

	uint16_t id_pagina = (*pagina)->id;
	//Convierte cada id a string y despues los concatena de 2 en 2
	char *nombre_archivo;
	nombre_archivo=concat_string(string_itoa(pid),string_itoa(id_segmento));
	nombre_archivo=concat_string(nombre_archivo,string_itoa(id_pagina));

	char* path;
	path=concat_string("en_disco/",nombre_archivo);
	path=concat_string(path,".txt");
	free(nombre_archivo);
	//crea un archivo y lo guarda en una carpeta interna.
	//el nombre se compone de pid, idsegmento y id pagina
	FILE* arch = fopen(path,"r");


	// ----Le consigo un marco a la pagina----
	marco_t* marco = buscar_marco_libre();
	if(marco==NULL)
	{
		swap_in(pagina, pid);
	}
	else
	{
		(*pagina)->marco=marco->id;
	}
	//Ahora pagina es de su marco
	(*pagina)->tiene_marco=true;
	(*pagina)->en_disco=false;
	//Ahora el marco es de su pagina
	marco = buscar_marco_segun_id((*pagina)->marco);
	marco->id_proceso=pid;
	marco->ocupado=true;
	fgets(marco->datos,256,arch);
	// ----Finalizo----

	remove(path);
	free(path);
	disminuyo_cantidad_archivos_swap();
}


