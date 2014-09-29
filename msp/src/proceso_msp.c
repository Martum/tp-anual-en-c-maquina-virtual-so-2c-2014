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

void quitar_segmento(proceso_msp_t *proceso, direccion base){
	bool _is_segmento_con_base(segmento_t *s) {
		return s->id == base; /* en realidad falta una conversion de id de segmento
							   * a direccion base.
							   * (concatenar pagina y desplazamiento en cero)
							   */
	}
	list_remove_and_destroy_by_condition(proceso->segmentos, (void*) _is_segmento_con_base, (void*)_destruye_segmento);
}

void _destruye_segmento(segmento_t *segmento){
	bool _is_pagina(pagina_t *pagina) {
		return true;
	}
	list_remove_and_destroy_by_condition(segmento->paginas, (void*)_is_pagina,(void*)_destruye_pagina);
	free(segmento);
}

void _destruye_pagina(pagina_t *pagina) {
	if(pagina->tiene_marco){
		// aca busco el marco y lo marco como no ocupado
		// .........
	}
	free(pagina);
}
