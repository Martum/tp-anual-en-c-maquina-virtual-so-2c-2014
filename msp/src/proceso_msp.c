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
#include "semaforos.h"

void listar_segmentos_de_un_proceso(proceso_msp_t *proceso){

	void _lista_segmentos(segmento_t *segmento) {
		printf("PID: %d \n", proceso->pid);
		printf("Nro segmento: %d \n", segmento->id);
		printf("Tamaño: %d \n", segmento->tamanio);
		printf("Dir virtual base de segmento: %x \n ------------------ \n", direccion_virtual_base_de_segmento(segmento->id));
		// agregar log de las cosas que hago printf
	}
	list_iterate(proceso->segmentos, (void*) _lista_segmentos);

}

proceso_msp_t* crear_proceso_msp(uint32_t un_pid){
	proceso_msp_t *proceso = malloc(sizeof(proceso_msp_t));
	proceso->pid = un_pid;

	//unlock_lista_procesos();
	list_add(get_lista_procesos(),proceso);
	//lock_lista_procesos();

	proceso->segmentos = list_create();
	return proceso;
}

proceso_msp_t* buscar_proceso_segun_pid(uint32_t pid){
	bool _es_proceso(proceso_msp_t* proceso) {
		return proceso->pid==pid;
	}

	//unlock_lista_procesos();

	 proceso_msp_t* proc = list_find(get_lista_procesos(), (void*) _es_proceso);

	//lock_lista_procesos();

	return proc;
}

bool quitar_segmento(proceso_msp_t *proceso, direccion base){
	bool puedo_quitar_segmento;

	bool _encuentro_segmento_con_base(segmento_t *s) {
		return direccion_virtual_base_de_segmento(s->id) == base;
	}

	// me fijo si encuentro el segmento
	if(list_any_satisfy(proceso->segmentos, (void*) _encuentro_segmento_con_base)){
		puedo_quitar_segmento = true;

		// remuevo los que cumplen la condicion
		list_remove_and_destroy_by_condition(proceso->segmentos, (void*) _encuentro_segmento_con_base, (void*)_destruye_segmento);
	}else{
		puedo_quitar_segmento = false;
	}

	return puedo_quitar_segmento;
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
	//lock_lista_indice_paginas();
	list_remove_and_destroy_element(get_indice_paginas(),pagina->id_en_indice,(void*)_destruye_pagina_de_indice);
	//unlock_lista_indice_paginas();
}

void _destruye_pagina_de_indice(pagina_t *pag) {
	free(pag);
}

