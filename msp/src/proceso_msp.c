/*
 * proceso_msp.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include "segmento.h"
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>

void listar_segmentos(proceso_msp_t *proceso){

	void _lista_segmentos(segmento_t *segmento) {
		printf("Segmento: %d \n", segmento->id);
		printf("Tamanio: %d \n ------------------ \n", segmento->tamanio);
	}
	list_iterate(proceso->segmentos, (void*) _lista_segmentos);

}

proceso_msp_t* crear_proceso_msp(){
	proceso_msp_t *proceso = malloc(sizeof(proceso_msp_t));
	proceso->segmentos = list_create();
	return proceso;
}
