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
#include <commons/string.h>

/*
uint32_t pid;
uint16_t id_segmento;




void swap_in(pagina_t* pagina)
{
	uint32_t direccion_marco_libre = liberar_un_marco();


	pagina->marco=direccion_marco_libre;



}


uint32_t liberar_un_marco()
{
	pagina_t* pagina_a_liberar= realizar_algoritmo_swapping(pid, id_segmento);


	uint32_t direccion_marco_a_liberar= pagina_a_liberar->marco;

	mover_a_disco(pagina_a_liberar, pid, id_segmento);



	return direccion_marco_a_liberar;
}


pagina_t* realizar_algoritmo_swapping(uint32_t pid, uint16_t id_segmento)
{


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




	txt_write_in_file(arch, string_itoa(pagina->marco));

	pagina->en_disco=true;

	pagina->tiene_marco=false;

}*/
