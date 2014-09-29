/*
 * pagina.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */
#include <stdbool.h>
#include "pagina.h"
#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include "segmento.h"
#include "configuraciones.h"

void crear_pagina(segmento_t *segmento){
	pagina_t *pagina = malloc(sizeof(pagina_t));
	pagina->tiene_marco = false;
	pagina->en_disco = false;

	agregar_pagina_a_segmento(segmento,pagina);
	pagina->id = list_size(segmento->paginas)-1;

	// agrego pagina a indice de paginas
	list_add(get_indice_paginas(), pagina);
}

void agregar_pagina_a_segmento(segmento_t *segmento, pagina_t *pagina){
	list_add(segmento->paginas, pagina);
}

direccion direccion_virtual_segmento_base_pagina(uint16_t id_segmento, uint16_t id_pagina)
{
	uint32_t base_segmento=direccion_virtual_base_de_segmento(id_segmento);
	return base_segmento | (id_pagina<<8);
}
