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

#include <commons/txt.h>
#include <commons/string.h>

uint32_t pid;
uint16_t id_segmento;


void swap_in(pagina_t* * pagina, uint32_t pid)
{
	marco_t* marco = liberar_un_marco();


	(*pagina)->marco=marco->id;

	marco->id_proceso = pid;
	marco->datos="";
	marco->ocupado=true;
}


uint32_t liberar_un_marco()
{
	//SACAR EL NULL
	pagina_t* pagina_a_liberar=NULL;//= realizar_algoritmo_swapping(pid, id_segmento);


	uint32_t direccion_marco_a_liberar= pagina_a_liberar->marco;

	mover_a_disco(pagina_a_liberar, pid, id_segmento);



	return direccion_marco_a_liberar;
}

//Debe deolver una pagina*
int realizar_algoritmo_swapping(uint32_t pid, uint16_t id_segmento)
{
	return 0;

}



void mover_a_disco(pagina_t* pagina, uint32_t pid, uint16_t id_segmento)
{

	//Convierte cada id a string y despues los concatena de 2 en 2
	char* nombre_archivo="";
	string_append(&nombre_archivo,string_itoa(pid));
	string_append(&nombre_archivo,string_itoa(id_segmento));
	string_append(&nombre_archivo,string_itoa(pagina->id));


	char* path="";
	string_append(&path,"en_disco/");
	string_append(&path,nombre_archivo);
	string_append(&path,".txt");

	//crea un archivo y lo guarda en una carpeta interna.
	//el nombre se compone de pid, idsegmento y id pagina
	FILE* arch = txt_open_for_append(path);

	//Consigo la direccion de donde voy a escribir los datos
	uint32_t direccion_logica = direccion_virtual_segmento_base_pagina(id_segmento,pagina->id);

	//Preparo el string que voy a escribir
	resultado_t* resultado = malloc(sizeof(resultado_t));
	char* string_a_escribir=leer_memoria(pid, direccion_logica, 256, resultado);
	free(resultado);

	//Escribo en el archivo
	txt_write_in_file(arch, string_a_escribir);

	pagina->en_disco=true;

	pagina->tiene_marco=false;

	txt_close_file(arch);

	aumento_cantidad_archivos_swap();
}

void swap_out(uint32_t pid, uint16_t id_segmento, uint16_t id_pagina)
{
	disminuyo_cantidad_archivos_swap();
}


