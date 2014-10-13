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
#include "configuraciones.h"
#include "direcciones.h"
#include "marco.h"

#include <commons/string.h>

direccion crear_segmento(uint32_t pid, uint32_t tamanio_en_bytes){
	// busco el proceso pid
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);

	// creo el segmento en la tabla de segmentos del proceso
	segmento_t* segmento = crear_segmento_con_paginas(proceso, tamanio_en_bytes);

	// creo la direccion virtual base del segmento
	direccion direccion_virtual = direccion_virtual_base_de_segmento(segmento->id);

	// retorno la direccion formada
	return direccion_virtual;
}

void destruir_segmento(uint32_t pid, direccion base){
	// busco el proceso pid
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);

	// saco el segmento del proceso y libero memoria
	quitar_segmento(proceso,base);
}


//Falta lanzar mensaje de error y leer la memoria propiamente dicha
char* leer_memoria(uint32_t pid, direccion direccion_logica, uint32_t tamanio)
{
	bool hay_error= memoria_invalida(pid, direccion_logica) || excede_limite_segmento(direccion_logica, tamanio);

	if(hay_error)
	{
		//LANZAR ERROR
	}
	else
	{
		uint32_t base_marco_datos = obtener_base_marco(pid, direccion_logica);

		return leer_marco(base_marco_datos, tamanio);

	}

	return 0;
}



void escribir_memoria(uint32_t pid, direccion direccon_logica, char* bytes_a_escribir, uint32_t tamanio){

}

