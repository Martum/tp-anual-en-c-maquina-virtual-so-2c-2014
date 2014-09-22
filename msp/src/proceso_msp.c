/*
 * proceso_msp.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>

#include "segmento.h"

#include <commons/collections/list.h>
#include "configuraciones.h"
#include "estructuras.h"

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

proceso_msp_t* buscar_proceso_segun_pid(uint32_t pid){
	bool _es_proceso(proceso_msp_t* proceso) {
		return proceso->pid==pid;
	}
	return list_find(get_lista_procesos(), (void*) _es_proceso);
}
