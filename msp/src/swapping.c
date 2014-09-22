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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>





void swap_in(pagina_t* pagina)
{
	uint32_t direccion_marco_libre = liberar_un_marco();


	pagina->marco=direccion_marco_libre;



}


uint32_t liberar_un_marco()
{
	pagina_t* pagina_a_liberar= realizar_algoritmo_swapping();


	uint32_t direccion_marco_a_liberar= pagina_a_liberar->marco;

	mover_a_disco(pagina_a_liberar);



	return direccion_marco_a_liberar;
}


pagina_t* realizar_algoritmo_swapping()
{


}



void mover_a_disco(pagina_t* pagina)
{
	//ACA FALTA DECIRLE A LA PAGINA QUE ACTUALMENTE OCUPA EL MARCO QUE VA A ESTAR EN DISCO
	//PAGINA.EN_DISCO=TRUE

	char* path = (char*)concat_string("en_disco/",/*direccion pagina como char* */"bla",".txt");

	//crea un archivo y lo guarda en una carpeta interna.
	//el nombre se compone de pid, idsegmento y id pagina
	FILE* arch = txt_open_for_append(path);

	//Como paso el contenido de la pagina a char*??
	txt_write_in_file(arch, /*contenido pagina*/"pagina");

	pagina->en_disco=true;

	pagina->tiene_marco=false;







}





char* concat_string(const char* a, const char* b, const char* c)
{
	char * aux = (char *) malloc(1+strlen(a)+strlen(b)+strlen(c));
	strcpy(aux,a);
	strcat(aux,b);
	strcat(aux, c);

	return aux;
}



