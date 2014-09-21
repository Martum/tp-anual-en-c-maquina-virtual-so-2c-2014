/*
 * swapping.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include "swapping.h"
#include "estructuras.h"
#include "marco.h"

#include <commons/txt.h>

uint32_t pid;

uint16_t id_segmento;

uint16_t id_pagina;


void swap_in(pagina_t* pagina)
{
	marco_t* marco_libre = liberar_un_marco();


	pagina->marco=marco_libre->base;
	marco_libre->ocupado=true;


}


 marco_t* liberar_un_marco()
{
	marco_t* marco_a_liberar= realizar_algoritmo_swapping();

	mover_a_disco(marco_a_liberar);

	return marco_a_liberar;
}


marco_t* realizar_algoritmo_swapping()
{


}



void mover_a_disco(marco_t* marco)
{
	//ACA FALTA DECIRLE A LA PAGINA QUE ACTUALMENTE OCUPA EL MARCO QUE VA A ESTAR EN DISCO
	//PAGINA.EN_DISCO=TRUE

	//Como mierda obtengo una pagina?

	//crea un archivo y lo guarda en una carpeta interna.
	//el nombre se compone de pid, idsegmento y id pagina
	FILE* arch = txt_open_for_append("en_disco/bla.txt");

	//txt_write_in_file(arch, "Estoy escribiendo!");



}



