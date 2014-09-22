/*
 * interfaz.c

 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdint.h>
#include "interfaz.h"
#include "estructuras.h"


direccion crear_segmento(uint32_t pid, uint32_t tamanio_en_bytes){
	// busco el proceso pid
//	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);

	// creo el segmento en la tabla de segmentos del proceso
//	segmento_t* segmento = crear_segmento_con_paginas(proceso, tamanio_en_bytes);


	return 0;
}

void destruir_segmento(uint32_t pid, direccion base){

}




