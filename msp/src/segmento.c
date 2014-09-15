/*
 * segmento.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include "segmento.h"
#include <stdint.h>
#include <stdbool.h>
#include "proceso_msp.h"
#include <commons/collections/list.h>
#include "estructuras.h"
#include "pagina.h"

int main(void){

	proceso_msp_t *proceso1 = crear_proceso_msp();

	segmento_t *segmento1 = crear_segmento(proceso1, 4);
	crear_segmento(proceso1, 2);
	crear_segmento(proceso1, 12);

	listar_segmentos(proceso1);
	listar_paginas(segmento1);

	return 0;
}


segmento_t *crear_segmento(proceso_msp_t *proceso, uint32_t tamanio) {
	segmento_t *segmento = malloc(sizeof(segmento_t));
	segmento->tamanio = tamanio;
	segmento->paginas = list_create();

	// creo las paginas en el segmento
	int i;
	for(i=0; i< tamanio; i++){
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
