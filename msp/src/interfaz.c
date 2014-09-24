/*
 * interfaz.c

 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "estructuras.h"
#include "proceso_msp.h"
#include "segmento.h"
#include <commons/bitarray.h>
#include <string.h>
#include "configuraciones.h"
#include <commons/string.h>
#include <inttypes.h>

direccion crear_segmento(uint32_t pid, uint32_t tamanio_en_bytes){
	// busco el proceso pid
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);

	// creo el segmento en la tabla de segmentos del proceso
	segmento_t* segmento = crear_segmento_con_paginas(proceso, tamanio_en_bytes);

	// la primer pagina empieza en 0 y el desplazamiento en la base es 0
	uint16_t primer_pagina = 0x000;
	int desplazamiento_a_la_base = 0x00;

	// creo la direccion virtual base del segmento
	uint32_t direccion_virtual = (segmento->id<<20) | (primer_pagina<<8)| desplazamiento_a_la_base;

	// retorno la direccion formada
	return direccion_virtual;

}

void destruir_segmento(uint32_t pid, direccion base){

}





