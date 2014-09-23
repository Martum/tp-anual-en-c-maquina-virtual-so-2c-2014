/*
 * marco.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */
#include <commons/collections/list.h>
#include "estructuras.h"
#include <stdlib.h>
#include <stdbool.h>

void crear_marco_en_lista_de_marcos(t_list *marcos, uint32_t base_de_marcos)
{
	marco_t *marco = malloc(sizeof(marco_t));
	marco->base = list_size(marcos)*256 + base_de_marcos;
	marco->id = list_size(marcos);
	marco->ocupado = false;
	list_add(marcos,marco);
}

/*
bool hay_marcos_libre(t_list *marcos)
{
	bool _marco_libre(marco_t marco)
	{

	}
}*/
