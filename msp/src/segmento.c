/*
 * segmento.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "proceso_msp.h"
#include "segmento.h"
#include "estructuras.h"
#include "pagina.h"

#include <commons/collections/list.h>
#include <commons/bitarray.h>

segmento_t *crear_segmento_con_paginas(proceso_msp_t *proceso, uint32_t tamanio_en_bytes) {
	segmento_t *segmento = malloc(sizeof(segmento_t));
	uint32_t paginas = cantidad_paginas(tamanio_en_bytes);
	segmento->tamanio = paginas;
	segmento->paginas = list_create();

	// creo las paginas en el segmento
	int i;
	for(i=0; i< paginas; i++){
		crear_pagina(segmento);
	}

	// agrego el segmento a la tabla de segmentos del proceso
	agregar_segmento_a_proceso(segmento,proceso);

    return segmento;
}

void agregar_segmento_a_proceso(segmento_t *segmento, proceso_msp_t *proceso){
	list_add(proceso->segmentos,segmento);
	segmento->id = list_size(proceso->segmentos)-1;
}

void listar_paginas(segmento_t *segmento){

	void _lista_paginas(pagina_t *pagina) {
		printf("Pagina: %d \n", pagina->id);
		printf("Tiene marco: %d \n ------------------ \n", pagina->tiene_marco);
	}
	list_iterate(segmento->paginas, (void*) _lista_paginas);
}

uint32_t cantidad_paginas(uint32_t tamanio_en_bytes){
	int agregado;
	if(tamanio_en_bytes % 256 == 0){
		agregado = 0;
	}else{
		agregado = 1;
	}
	return tamanio_en_bytes / 256 + agregado;
}

direccion direccion_virtual_base_de_segmento(uint16_t id_segmento){
	// la primer pagina empieza en 0 y el desplazamiento en la base es 0
	uint16_t primer_pagina = 0x000;
	int desplazamiento_a_la_base = 0x00;
	// retorno la direccion (32 bits)
	return (id_segmento<<20) | (primer_pagina<<8)| desplazamiento_a_la_base;
}

segmento_t* buscar_segmento_segun_id_en_lista_segmentos(uint16_t id_segmento, t_list* lista_segmentos){
	bool _es_segmento(segmento_t* segmento) {
		return segmento->id==id_segmento;
	}
	return list_find(lista_segmentos, (void*) _es_segmento);
}
