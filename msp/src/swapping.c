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

#include <commons/txt.h>
#include <commons/string.h>




void swap_in(pagina_t* * pagina, uint32_t pid)
{
	marco_t* marco = liberar_un_marco();


	//Modifico la pagina y el marco para que se relacionen entre si
	(*pagina)->marco=marco->id;
	marco->id_proceso = pid;
	marco->datos="";
	marco->ocupado=true;
}

//TODO Este pid e id_segmento no son los que deberian
marco_t* liberar_un_marco()
{
	uint16_t id_pag_swap;
	pagina_t* pagina_a_liberar;
	segmento_t* segmento_contenedor;
	//SACAR EL NULL
	uint32_t id_marco_a_liberar= realizar_algoritmo_swapping(&id_pag_swap);




	marco_t* marco_a_liberar= buscar_marco_segun_id(id_marco_a_liberar);


	obtener_segmento_y_pagina(&pagina_a_liberar, &segmento_contenedor, id_pag_swap, marco_a_liberar->id);



	mover_a_disco(&pagina_a_liberar, marco_a_liberar->id_proceso, segmento_contenedor->id);



	return marco_a_liberar;
}

//Debe deolver una pagina*
uint32_t realizar_algoritmo_swapping(uint16_t * id_pagina_swap)
{

	char* algoritmo=algoritmo_sustitucion_de_paginas();
	uint32_t id_marco;
	char* clock="CLOCK";
	if(algoritmo==clock)//TODO bug, no considera iguales
	{
		id_marco=algoritmo_clock(id_pagina_swap);
	}
	else
	{
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
	char* nombre_archivo="";
	string_append(&nombre_archivo,string_itoa(pid));
	string_append(&nombre_archivo,string_itoa(id_segmento));
	string_append(&nombre_archivo,string_itoa((*pagina)->id));



	char* path="";
	string_append(&path,"en_disco/");
	string_append(&path,nombre_archivo);
	string_append(&path,".txt");

	//crea un archivo y lo guarda en una carpeta interna.
	//el nombre se compone de pid, idsegmento y id pagina
	FILE* arch = txt_open_for_append(path);

	//Consigo la direccion de donde voy a escribir los datos
	uint32_t direccion_logica = direccion_virtual_segmento_base_pagina(id_segmento,(*pagina)->id);

	//Preparo el string que voy a escribir
	resultado_t* resultado = malloc(sizeof(resultado_t));
	char* string_a_escribir=leer_memoria(pid, direccion_logica, 256, resultado);
	free(resultado);

	//Escribo en el archivo
	txt_write_in_file(arch, string_a_escribir);

	(*pagina)->en_disco=true;

	(*pagina)->tiene_marco=false;

	txt_close_file(arch);

	aumento_cantidad_archivos_swap();
}

void swap_out(uint32_t pid, uint16_t id_segmento, uint16_t id_pagina)
{
	disminuyo_cantidad_archivos_swap();
}


