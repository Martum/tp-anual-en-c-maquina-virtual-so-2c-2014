/*
 * interfaz.c

 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdint.h>
#include <stdlib.h>
#include "estructuras.h"
#include "proceso_msp.h"
#include "segmento.h"
#include <commons/bitarray.h>
direccion crear_segmento(uint32_t pid, uint32_t tamanio_en_bytes){
	// busco el proceso pid
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);

	// creo el segmento en la tabla de segmentos del proceso
	segmento_t* segmento = crear_segmento_con_paginas(proceso, tamanio_en_bytes);

	// creo la direccion virtual base del segmento
	char *dir_virtual = malloc(2);
	t_bitarray* direccion_virtual = bitarray_create(dir_virtual, 8);
	char* direccion_segmento = arma_direccion(segmento->id,12);
	int i;
	for(i=0; i<12; i++){
		if(direccion_segmento[i]=='1'){
			bitarray_set_bit(direccion_virtual, i);
		}
	}

	// retorno la direccion virtual base del segmento
	return (uint32_t)direccion_virtual->bitarray;
}

void destruir_segmento(uint32_t pid, direccion base){

}





