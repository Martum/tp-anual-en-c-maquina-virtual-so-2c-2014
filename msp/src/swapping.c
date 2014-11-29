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



void swap_in(pagina_t* * pagina, uint32_t id_segmento, uint32_t pid)
{
	marco_t* marco = liberar_un_marco();

	//Modifico la pagina y el marco para que se relacionen entre si
	(*pagina)->marco=marco->id;
	(*pagina)->tiene_marco=true;
	marco->id_proceso = pid;
	marco->ocupado=true;

	loggear_trace("Swap in pagina %d del segmento %d del proceso %d en marco %d.",
			(*pagina)->id, id_segmento, marco->id_proceso, marco->id);
	loggear_trace("Se asigno el marco %d al proceso %d.", marco->id, marco->id_proceso);

	if(cantidad_marcos_libre() == 0){
		loggear_info("Espacio de memoria principal lleno");
	}
}


marco_t* liberar_un_marco()
{
	uint16_t id_pag_swap;
	pagina_t* pagina_a_liberar;
	segmento_t* segmento_contenedor;
	//SACAR EL NULL
	uint32_t id_marco_a_liberar= realizar_algoritmo_swapping(&id_pag_swap);

	marco_t* marco_a_liberar= buscar_marco_segun_id(id_marco_a_liberar);
	loggear_trace("Se libero el marco %d.", marco_a_liberar->id);

	obtener_segmento_y_pagina(&pagina_a_liberar, &segmento_contenedor, id_marco_a_liberar, marco_a_liberar->id_proceso);

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

void obtener_segmento_y_pagina(pagina_t* * pagina_swap,segmento_t* * segmento_contenedor, uint32_t id_marco_swap, uint32_t pid)
{
	proceso_msp_t* proceso_contenedor = buscar_proceso_segun_pid(pid);
	int i, j;
	for(i=0; i<list_size(proceso_contenedor->segmentos); i++){
		segmento_t* segment = list_get(proceso_contenedor->segmentos, i);
		for(j=0; j<list_size(segment->paginas); j++){
			pagina_t* pagi = list_get(segment->paginas, j);
			if(pagi->tiene_marco && pagi->marco == id_marco_swap){
				(*pagina_swap) = pagi;
				(*segmento_contenedor) = segment;
			}
		}
	}

}



void mover_a_disco(pagina_t* * pagina, uint32_t pid, uint16_t id_segmento)
{
	//Convierte cada id a string y despues los concatena de 2 en 2
	char *nombre_archivo=generar_nombre_archivo_swap(pid, id_segmento, (*pagina)->id);

	char* path;
	path=concat_string("en_disco/",nombre_archivo);
	path=concat_string(path,".txt");
	free(nombre_archivo);

	//crea un archivo y lo guarda en una carpeta interna.
	//el nombre se compone de pid, idsegmento y id pagina
	FILE* arch = fopen(path, "w");
	free(path);

	marco_t* marco= buscar_marco_segun_id((*pagina)->marco);
	//Preparo el string que voy a escribir

	//Escribo en el archivo
	fwrite(marco->datos, sizeof(char), 256, arch);

	(*pagina)->en_disco=true;
	(*pagina)->tiene_marco=false;

	fclose(arch);

	aumento_cantidad_archivos_swap();

	if(get_cantidad_archivos_swap() == cantidad_swap() * 4096){
		loggear_info("Espacio de swap lleno");
	}
}

void swap_out(uint32_t pid, uint16_t id_segmento, pagina_t* * pagina)
{
	// ----Abro el archivo----
	uint16_t id_pagina = (*pagina)->id;
	//Convierte cada id a string y despues los concatena de 2 en 2
	char *nombre_archivo=generar_nombre_archivo_swap(pid, id_segmento, id_pagina);

	char* path;
	path=concat_string("en_disco/",nombre_archivo);
	path=concat_string(path,".txt");
	free(nombre_archivo);
	//crea un archivo y lo guarda en una carpeta interna.
	//el nombre se compone de pid, idsegmento y id pagina
	FILE* arch = fopen(path,"r");

	// ----Le consigo un marco a la pagina----
	marco_t* marco = buscar_marco_libre();

	if(marco==NULL){
		swap_in(pagina, id_segmento, pid);
	}else{
		(*pagina)->marco=marco->id;
	}

	//Ahora pagina es de su marco
	(*pagina)->tiene_marco=true;
	(*pagina)->en_disco=false;

	//Ahora el marco es de su pagina
	marco = buscar_marco_segun_id((*pagina)->marco);
	marco->id_proceso=pid;
	(*pagina)->marco=marco->id;
	marco->ocupado=true;
	fread(marco->datos,sizeof(char),256,arch);

	// ----Finalizo----
	remove(path);
	free(path);

	disminuyo_cantidad_archivos_swap();

	loggear_trace("Swap out pagina %d del segmento %d del proceso %d.", id_pagina, id_segmento, pid);
}


char* generar_nombre_archivo_swap(uint32_t pid, uint16_t id_segmento, uint16_t id_pagina)
{
	char *nombre_archivo;
	nombre_archivo=concat_string(string_itoa(pid),string_itoa(id_segmento));
	nombre_archivo=concat_string(nombre_archivo,string_itoa(id_pagina));
	return nombre_archivo;
}

void destruir_archivos_swapp_proceso(uint32_t pid, segmento_t* segmento)
{
	int i;
	char* nombre_archivo;
	for(i=0;i<list_size(segmento->paginas);i++)
	{
		pagina_t* pagina=list_get(segmento->paginas,i);
		if(pagina->en_disco)
		{
			nombre_archivo=generar_nombre_archivo_swap(pid, segmento->id, pagina->id);
			char* path;
			path=concat_string("en_disco/",nombre_archivo);
			path=concat_string(path,".txt");
			free(nombre_archivo);
			remove(path);
			free(path);

		}
	}
}
