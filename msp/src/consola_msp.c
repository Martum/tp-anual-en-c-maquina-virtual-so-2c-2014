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
#include <commons/bitarray.h>

#include "interfaz.h"
#include "consola_msp.h"
#include "configuraciones.h"

int main(void){
	inicializar_lista_procesos();
	inicializar_memoria_fisica_total(10000);// el parametro viene por archivo de configuracion

	char *dir_virtual = malloc(2);
	t_bitarray* direccion_virtual = bitarray_create(dir_virtual, 8);

	char* direccion_segmento = "001100100101"; // = ...
	int i;
	for(i=0; i<12; i++){
		if(direccion_segmento[i]=='1'){
			bitarray_set_bit(direccion_virtual, i);
		}
		if(direccion_segmento[i]=='1'){
			printf("%d",1);
		}else if(direccion_segmento[i]=='0'){
			printf("%d",0);
		}

	}

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
