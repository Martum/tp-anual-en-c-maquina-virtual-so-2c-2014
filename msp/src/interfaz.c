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

	// creo la direccion virtual base del segmento
	char* direccion_segmento_en_binario = arma_direccion(segmento->id,12);

	/* le concateno 12 ceros que referencian a la pagina 0 y 8 ceros mas
	* por desplazamiento cero
	*/
	char* aux=malloc(2);
	int j;
	aux=string_itoa(0);
	for(j=0; j<20; j++){
		string_append(&direccion_segmento_en_binario,aux);
	}

	// libero memoria de la variable auxiliar
	free(aux);

	// puedo retornar la direccion base del segmento como una cadena de ceros y unos
	// HAY QUE CONVERTIR ESA CADENA DE CEROS Y UNOS A NUMERO
	return 0;

}

void destruir_segmento(uint32_t pid, direccion base){

}





