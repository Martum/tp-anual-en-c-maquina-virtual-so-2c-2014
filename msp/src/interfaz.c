/*
 * interfaz.c

 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "estructuras.h"
#include "proceso_msp.h"
#include "segmento.h"
#include "configuraciones.h"
#include "direcciones.h"
#include "marco.h"
#include "pagina.h"


#include <commons/string.h>
#include <hu4sockets/resultados.h>

direccion crear_segmento(uint32_t pid, uint32_t tamanio_en_bytes, resultado_t *resultado){
	// busco el proceso pid
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);

	// calculo cuantas paginas necesita el segmento
	uint32_t cant_paginas = cantidad_paginas(tamanio_en_bytes);

	bool hay_memoria = puedo_crear_paginas(tamanio_en_bytes, cant_paginas);

	if(hay_memoria){
		*(resultado) = RESULTADO_OK;
	}else{
		*(resultado) = ERROR_DE_MEMORIA_LLENA;
	}

	// creo el segmento en la tabla de segmentos del proceso
	segmento_t* segmento = crear_segmento_con_paginas(proceso, cant_paginas);

	// creo la direccion virtual base del segmento
	direccion direccion_virtual = direccion_virtual_base_de_segmento(segmento->id);

	// retorno la direccion formada
	return direccion_virtual;
}

void destruir_segmento(uint32_t pid, direccion base, resultado_t *resultado){
	// busco el proceso pid
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);

	// saco el segmento del proceso y libero memoria
	bool ok = quitar_segmento(proceso,base);

	if(ok){
		*(resultado) = RESULTADO_OK;
	}else{
		*(resultado) = ERROR_NO_ENCUENTRO_SEGMENTO;
	}
}


//Falta lanzar mensaje de error y leer la memoria propiamente dicha
char* leer_memoria(uint32_t pid, direccion direccion_logica, uint32_t tamanio,
		resultado_t *resultado){

	//Estan inicializados con verdura para que no tire warnings
	//En la siguiente funcion se le asignas los valores correctos
	proceso_msp_t* proceso=NULL;
	segmento_t* segmento=NULL;
	pagina_t* pagina=NULL;
	uint16_t desplazamiento=0;

	bool memoria_invalida = descomposicion_direccion_logica(direccion_logica,pid,proceso,segmento,pagina,desplazamiento);

	bool hay_error= memoria_invalida || excede_limite_segmento(proceso, segmento, pagina, desplazamiento, tamanio);

	char* datos="";
	if(hay_error)
	{
		//LANZAR ERROR
		*(resultado) = SEGMENTATION_FAULT;
	}
	else
	{
		*(resultado) = RESULTADO_OK;


		bool mas_paginas = true;
		uint16_t desplazamiento = div(direccion_logica,0x100).rem;
		marco_t* marco = buscar_marco_segun_id(pagina->marco);
		pagina->bit_referencia=1;
		while((tamanio==0)&&(mas_paginas))
		{
			//Esta funcion va cambiando el TAMANIO asique nunca va a volver a ser el mismo
			string_append(&datos, leer_marco(marco->datos, desplazamiento,tamanio, mas_paginas));

			//En este punto ya lei todo lo que podia del marco y debo buscar el siguiente
			pagina_t* pagina_siguiente = siguiente_pagina(pagina->id, segmento->paginas);
			marco = buscar_marco_segun_id(pagina_siguiente->marco);
		}


	}

	return datos;
}



void escribir_memoria(uint32_t pid, direccion direccion_logica,
		char* bytes_a_escribir, uint32_t tamanio, resultado_t *resultado){

	//Estan inicializados con verdura para que no tire warnings
	//En la siguiente funcion se le asignas los valores correctos
	proceso_msp_t* proceso=NULL;
	segmento_t* segmento=NULL;
	pagina_t* pagina=NULL;
	uint16_t desplazamiento=0;

	bool memoria_invalida = descomposicion_direccion_logica(direccion_logica,pid,proceso,segmento,pagina,desplazamiento);

	bool hay_error = memoria_invalida || excede_limite_segmento(proceso, segmento, pagina, desplazamiento, tamanio);


	if(hay_error)
	{
		//LANZAR ERROR
		*(resultado) = SEGMENTATION_FAULT;
	}
	else
	{
		*(resultado) = RESULTADO_OK;

		//NO CAMBIA EL MARCO TERRIBLE BUG
		bool mas_paginas = true;
		uint16_t desplazamiento = div(direccion_logica,0x100).rem;
		marco_t* marco = buscar_marco_segun_id(pagina->marco);
		pagina->bit_referencia=1;
		while((tamanio==0)&&(mas_paginas))
		{
			//Esta funcion va cambiando el TAMANIO asique nunca va a volver a ser el mismo
			escribir_marco(marco->datos, desplazamiento,tamanio, bytes_a_escribir, mas_paginas);

			//En este punto ya lei todo lo que podia del marco y debo buscar el siguiente
			pagina_t* pagina_siguiente = siguiente_pagina(pagina->id, segmento->paginas);
			marco = buscar_marco_segun_id(pagina_siguiente->marco);
		}


	}

}

