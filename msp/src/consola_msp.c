/*
 * consola_msp.c
 *
 *  Created on: 17/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdbool.h>

#include <commons/collections/list.h>

#include "estructuras.h"
#include "pagina.h"
#include "segmento.h"
#include "proceso_msp.h"
#include "interfaz.c"
#include "consola_msp.h"

int main(void){

	lista_procesos = list_create();
	memoria_fisica_total = malloc(10000); // el parametro viene por archivo de configuracion

	return 0;
}
/* Prueba de main para saber que anda
proceso_msp_t *proceso1 = crear_proceso_msp();

	segmento_t *segmento1 = crear_segmento(proceso1, 400);
	crear_segmento(proceso1, 200);
	crear_segmento(proceso1, 1200);
	crear_segmento(proceso1, 256);

	listar_segmentos(proceso1);
	listar_paginas(segmento1);
*/
