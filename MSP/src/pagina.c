/*
 * pagina.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */
#include <stdbool.h>
#include "pagina.h"
#include <stdio.h>
#include <commons/collections/list.h>




int main(void) {
	t_list* lista = list_create();
	list_add(lista,"hola");
	list_add(lista,"puto");
	list_add(lista,"meh");

	printf("%d",list_size(lista));

	return 0;
}



//CAMBIAR LO QUE DEVUELVE
void crear_pagina(segmento_t segmento)
{
	pagina_t pagina;
	pagina.tiene_marco = false;
	pagina.en_disco = false;


	agregar_pagina_a_segmento(segmento,pagina);
	pagina.id = list_size(segmento.paginas)-1;


}

//CAMBIAR LO QUE DEVUELVE
void agregar_pagina_a_segmento(segmento_t segmento, pagina_t pagina)
{
	list_add(segmento.paginas, &pagina);

}
