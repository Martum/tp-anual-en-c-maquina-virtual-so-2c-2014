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
#include "marco.h"

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
		return direccion_virtual_base_de_segmento(s->id) == base;
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
	// marco como desocupado su marco si es que tiene
	if(pagina->tiene_marco){
		marco_t* m = buscar_marco_segun_id(pagina->marco);
		m->ocupado = false;
	}
	bool _is_pagina(pagina_t *pagina){
		return true;
	}
	// saco a la pagina de la lista indice de paginas
	list_remove_and_destroy_element(get_indice_paginas(),pagina->id_en_indice,(void*)_destruye_pagina_de_indice);
//	list_remove_and_destroy_by_condition(get_indice_paginas(), (void*)_is_pagina, (void*)_destruye_pagina_de_indice);

	// libero memoria de la pagina
	free(pagina);
}

void _destruye_pagina_de_indice(pagina_t *pag) {
	free(pag);
}

