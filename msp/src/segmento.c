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
#include "marco.h"

#include <commons/collections/list.h>
#include <commons/bitarray.h>

segmento_t* crear_segmento_con_paginas(proceso_msp_t *proceso, uint32_t cant_paginas) {
	segmento_t *segmento = malloc(sizeof(segmento_t));
	segmento->tamanio = cant_paginas;
	segmento->paginas = list_create();

	// creo las paginas en el segmento
	int i;
	for(i=0; i< cant_paginas; i++){
		crear_pagina(segmento);
	}

	// agrego el segmento a la tabla de segmentos del proceso
	agregar_segmento_a_proceso(segmento,proceso);

    return segmento;
}

void agregar_segmento_a_proceso(segmento_t *segmento, proceso_msp_t *proceso){
	segmento_t* segmento_auxiliar = (segmento_t*)list_take(proceso->segmentos,list_size(proceso->segmentos));
	segmento->id = (segmento_auxiliar->id)+1;
	list_add(proceso->segmentos,segmento);
}

void listar_paginas_de_un_segmento(segmento_t *segmento){

	void _lista_paginas(pagina_t *pagina) {
		printf("Nro segmento: %d \n", segmento->id);
		printf("Nro pagina: %d \n", pagina->id);
		if(pagina->en_disco){
			printf("La pagina esta en: %s \n ------------------ \n", "disco");
		}else{
			printf("La pagina esta en: %s \n ------------------ \n", "memoria");
		}
	}
	list_iterate(segmento->paginas, (void*) _lista_paginas);
	// falta agregar log de todo lo mismo que hago printf
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

bool puedo_crear_paginas(uint32_t tamanio_en_bytes, uint32_t cant_paginas){

	uint32_t cant_marcos_disponibles = cantidad_marcos_libre();

	if(cant_paginas <= cant_marcos_disponibles){
		return true;
	}

/*	uint32_t bytes_no_cubiertos_por_marcos =
			tamanio_en_bytes - (cant_marcos_disponibles*256);
*/

	// FALTA UNA FUNCION QUE CALCULE CUANTOS BYTES TIENEN LOS ARCHIVOS DE SWAṔ
	// 1048576 bytes por cada mb seteado para swap en arch de configuracion


	return true;
}
