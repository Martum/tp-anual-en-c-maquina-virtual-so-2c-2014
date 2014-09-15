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
#include "pagina.h"
#include "proceso_msp.h"
#include <commons/collections/list.h>

int main(void){
	proceso_msp_t *proceso1 = crear_proceso_msp();

	crear_segmento(proceso1, 4, false);
	crear_segmento(proceso1, 2, false);
	crear_segmento(proceso1, 12, true);

	listar_segmentos(proceso1);

	return 0;
}


segmento_t *crear_segmento(proceso_msp_t *proceso, uint32_t tamanio, bool acceso_usuario) {
	segmento_t *segmento = malloc(sizeof(segmento_t));
	segmento->tamanio = tamanio;
	segmento->acceso_usuario = acceso_usuario;
	segmento->paginas = list_create();
	// creo las paginas en el segmento
/*	int i;
	for(i=0; i< tamanio; i++){
	//	crear_pagina(segmento);
	}
*/
	// agrego el segmento a la tabla de segmentos del proceso
	agregar_segmento_a_proceso(segmento,proceso);

    return segmento;
}

void agregar_segmento_a_proceso(segmento_t *segmento, proceso_msp_t *proceso){
	list_add(proceso->segmentos,segmento);
	segmento->id = list_size(proceso->segmentos)-1;
}



/*
	proc = crear_proceso(....)
	crear_segmento(proc, tamanio, false)

*/
