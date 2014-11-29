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
#include <string.h>

#include "estructuras.h"
#include "proceso_msp.h"
#include "segmento.h"
#include "configuraciones.h"
#include "direcciones.h"
#include "marco.h"
#include "pagina.h"
#include "algoritmos_sustitucion.h"
#include "funciones_streams.h"
#include "logs.h"
#include "swapping.h"

#include <commons/string.h>
#include <hu4sockets/resultados.h>


direccion crear_segmento(uint32_t pid, uint32_t tamanio_en_bytes, resultado_t *resultado){
	// busco el proceso pid
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);

	if(proceso == NULL){
		proceso = crear_proceso_msp(pid);
	}

	// calculo cuantas paginas necesita el segmento
	uint32_t cant_paginas = cantidad_paginas(tamanio_en_bytes);

	bool hay_memoria = puedo_crear_paginas(tamanio_en_bytes, cant_paginas);

	if(hay_memoria){
		*(resultado) = RESULTADO_OK;
	}else{
		*(resultado) = ERROR_DE_MEMORIA_LLENA;
	}

	// creo el segmento en la tabla de segmentos del proceso
	segmento_t* segmento = crear_segmento_con_paginas(proceso, cant_paginas, tamanio_en_bytes);

	// creo la direccion virtual base del segmento
	direccion direccion_virtual = direccion_virtual_base_de_segmento(segmento->id);

	if(*(resultado)== RESULTADO_OK){
		loggear_trace("Se creo el segmento correctamente para el proceso %d.", pid);
	}else{
		loggear_trace("No pudo crearse el segmento solicitado por el proceso %d.", pid);
	}

	// retorno la direccion formada
	return direccion_virtual;
}

void destruir_segmento(uint32_t pid, direccion base, resultado_t *resultado){

	bool _es_proceso(proceso_msp_t* proceso) {
		return proceso->pid==pid;
	}

	void _destruyo_proc(proceso_msp_t* proceso){
		free(proceso);
	}

	void _destruyo_proceso(proceso_msp_t* proceso, direccion base, bool* ok)
	{
		int i;
		for(i=0; i<list_size(proceso->segmentos); i++)
		{
			segmento_t* segmento=list_get(proceso->segmentos, i);
			destruir_archivos_swapp_proceso(pid, segmento);
		}
		(*ok) = quitar_segmento(proceso, base);
		list_destroy(proceso->segmentos);
		liberar_marcos_proceso(proceso->pid);

		list_remove_and_destroy_by_condition(get_lista_procesos(), (void*) _es_proceso, (void*) _destruyo_proc);
	}

	bool ok;

	if(list_any_satisfy(get_lista_procesos(),(void*) _es_proceso)){
		// busco el proceso pid
		proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);

		if(list_size(proceso->segmentos)==1)
		{
			proceso_msp_t* proc = list_find(get_lista_procesos(), (void*) _es_proceso);
			//proceso_msp_t* proc = list_get(get_lista_procesos(),(proceso->pid)-1);
			_destruyo_proceso(proc, base, &ok);
		}
		else
		{
			ok = quitar_segmento(proceso,base);
		}

	}else{
		ok = false;
	}

	if(ok){
		*(resultado) = RESULTADO_OK;
		loggear_trace("Se destruyo correctamente el segmento %x del proceso %d.", base>>20, pid);
	}else{
		*(resultado) = ERROR_NO_ENCUENTRO_SEGMENTO;
		loggear_trace("No pudo destruirse el segmento %x del proceso %d.", base>>20, pid);
	}
}

char* leer_memoria(uint32_t pid, direccion direccion_logica, uint32_t tamanio,resultado_t *resultado)
{
	//Estan inicializados con verdura para que no tire warnings
	//En la siguiente funcion se le asignas los valores correctos
	proceso_msp_t* proceso=NULL;//malloc(sizeof(proceso_msp_t));
	segmento_t* segmento=NULL;//malloc(sizeof(segmento_t));
	pagina_t* pagina=NULL;//malloc(sizeof(pagina_t));
	uint16_t desplazamiento=0;
	pagina_t* pagina_siguiente=NULL;
	uint16_t id_pagina_siguiente;

	bool memoria_invalida = descomposicion_direccion_logica(direccion_logica,pid,&proceso,&segmento,&pagina,&desplazamiento);

	bool hay_error= memoria_invalida || excede_limite_segmento(proceso, segmento, pagina, desplazamiento, tamanio);

	char* datos = malloc(0);
	int32_t tam_restante = tamanio;
	int32_t tam_datos = 0;
	if(hay_error)
	{
		*(resultado) = SEGMENTATION_FAULT;
	}
	else
	{
		*(resultado) = RESULTADO_OK;

		//Con bool no anda, int cumple el proposito
		int mas_paginas = true;

		marco_t* marco = buscar_marco_segun_id(pagina->marco);
		set_bit_referencia(pagina);

		if(string_equals_ignore_case(algoritmo_sustitucion_de_paginas(), "LRU")){
			ubico_al_principio(pagina);
		}

		while((tamanio!=0)&&(mas_paginas))
		{
			//Esta funcion va cambiando el TAMANIO asique nunca va a volver a ser el mismo.
			char* txt= leer_marco(marco->datos, desplazamiento,&tamanio, &mas_paginas, (pagina->max_modificable));
			//TODO Cambiar esta funcion katinga por la de las commons
			//TODO Sacar la funcion katinga de marco.c

			int32_t tam_a_copiar = tam_restante - tamanio;
			char* datos_aux = malloc(tam_datos + tam_a_copiar);
			memcpy(datos_aux, datos, tam_datos);
			free(datos);
			memcpy(datos_aux + tam_datos, txt, tam_a_copiar);
			datos = datos_aux;
			tam_datos = tam_datos + tam_a_copiar;
			tam_restante = tam_restante - tam_a_copiar;

			free(txt);
			//Aunque haya o no más paginas, despues de una lectura no va a haber más desplazamiento.
			desplazamiento=0;

			//En este punto ya lei todo lo que podia del marco y debo buscar el siguiente.
			id_pagina_siguiente= (pagina->id);
			hay_siguiente_pagina(id_pagina_siguiente, segmento->paginas,&pagina_siguiente);
			if((pagina_siguiente!=NULL) && mas_paginas)
			{
				if(!(pagina_siguiente->tiene_marco))
				{
					asignar_marco(&pagina_siguiente, &segmento, pid);
				}
				marco = buscar_marco_segun_id(pagina_siguiente->marco);
				pagina = pagina_siguiente;
			}
		}
	}
	return datos;
}

void escribir_memoria(uint32_t pid, direccion direccion_logica,char* bytes_a_escribir, uint32_t tamanio, resultado_t *resultado)
{
	//Estan inicializados con verdura para que no tire warnings
	//En la siguiente funcion se le asignas los valores correctos
	proceso_msp_t* proceso=NULL;
	segmento_t* segmento=NULL;
	pagina_t* pagina=NULL;
	marco_t* marco= NULL;
	uint16_t desplazamiento=0;
	pagina_t* pagina_siguiente=NULL;
	uint16_t id_pagina;

	bool memoria_invalida = descomposicion_direccion_logica(direccion_logica,pid,&proceso,&segmento,&pagina,&desplazamiento);

	bool hay_error = memoria_invalida || excede_limite_segmento(proceso, segmento, pagina, desplazamiento, tamanio);

	if(hay_error)
	{
		//LANZAR ERROR
		*(resultado) = SEGMENTATION_FAULT;
	}
	else
	{
		*(resultado) = RESULTADO_OK;

		//Sin razon aparente anda mal con el tipo bool, asique tiro int
		int mas_paginas = 1;
		marco = buscar_marco_segun_id(pagina->marco);
		set_bit_referencia(pagina);

		if(string_equals_ignore_case(algoritmo_sustitucion_de_paginas(), "LRU")){
			ubico_al_principio(pagina);
		}

		uint32_t cantidad_escrito_acumulada = 0;
		while((tamanio!=0)&&(mas_paginas))
		{
			//Esta funcion va cambiando el TAMANIO asique nunca va a volver a ser el mismo
			escribir_marco(&marco, desplazamiento,&tamanio, &bytes_a_escribir, &mas_paginas, (pagina->max_modificable), &cantidad_escrito_acumulada);

			//Aunque haya o no más paginas, despues de una lectura no va a haber más desplazamiento.
			desplazamiento=0;

			//En este punto ya lei todo lo que podia del marco y debo buscar el siguiente
			id_pagina= (pagina->id);
			hay_siguiente_pagina(id_pagina,segmento->paginas,&pagina_siguiente);
			if((pagina_siguiente!=NULL) && mas_paginas)
			{
				if(!(pagina_siguiente->tiene_marco))
				{
					asignar_marco(&pagina_siguiente, &segmento, pid);
				}

				marco = buscar_marco_segun_id(pagina_siguiente->marco);
				pagina=pagina_siguiente;
			}

		}


	}

}

