/*
 * consola_msp.c
 *
 *  Created on: 17/09/2014
 *      Author: utnso
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "estructuras.h"
#include "consola_msp.h"
#include "proceso_msp.h"
#include "configuraciones.h"
#include "algoritmos_sustitucion.h"
#include "interfaz.h"
#include "segmento.h"
#include "semaforos.h"

#include <commons/string.h>
// #include <commons/log.h>

#include <hu4sockets/resultados.h>

void* escuchar_consola_msp(void* otro_ente){
	printf("Bienvenido a la MSP! \n\n");
	printf("Instrucciones disponibles: \n");
	printf(" - Crear Segmento: pid, tamaño \n");
	printf(" - Destruir Segmento: pid, dir_base \n");
	printf(" - Escribir Memoria: pid, dir_virtual, tamaño, texto \n");
	printf(" - Leer Memoria: pid, dir_virtual, tamaño \n");
	printf(" - Tabla de Segmentos \n");
	printf(" - Tabla de Paginas: pid \n");
	printf(" - Listar Marcos \n\n");
	printf("Para cerrar la consola escriba \"Cerrar\" \n\n");
	printf("Esperando instrucciones ... \n\n");

	_dar_instrucciones_por_consola();

	return NULL;
}


void _dar_instrucciones_por_consola(){
	char cadena[350] = "";

	scanf("%[^\n]%*c",cadena);

	if(strcmp(cadena,"Cerrar")== 0){
		printf("Cerrando consola MSP ... \n");
		return;
	}else{
		// printf("la cadena es: %s \n", cadena);

		int resultado = _matcheo_cadena_con_funcion(cadena);

		if(!resultado){
			printf("Instruccion invalida \n");
		}

		_dar_instrucciones_por_consola();
	}
}

bool _matcheo_cadena_con_funcion(char *cadena){

	// obtengo el nombre de la funcion
	char** funcion = string_split(cadena, ":");

 	char* nombre_funcion = funcion[0];
 	char* lista_parametros = funcion[1];

 	bool matchea = false;

	if(strcmp(nombre_funcion,"Crear Segmento")==0){
		char** parametros = string_split(lista_parametros, ",");
		crear_segment(_parametro_int(parametros[0]), _parametro_int(parametros[1]));
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Destruir Segmento")==0){
		char** parametros = string_split(lista_parametros, ",");
		destruir_segment(_parametro_int(parametros[0]), _parametro_int(parametros[1]));
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Escribir Memoria")==0){
		char** parametros = string_split(lista_parametros, ",");
		escribir_memo(_parametro_int(parametros[0]),
						_parametro_int(parametros[1]),
						_parametro_int(parametros[2]),
						_parametro_string(parametros[3]));
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Leer Memoria")==0){
		char** parametros = string_split(lista_parametros, ",");
		leer_memo(_parametro_int(parametros[0]),
					_parametro_int(parametros[1]),
					_parametro_int(parametros[2]));
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Tabla de Segmentos")==0){
		tabla_segmentos();
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Tabla de Paginas")==0){
		char** parametros = string_split(lista_parametros, ",");
		tabla_paginas(_parametro_int(parametros[0]));
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Listar Marcos")==0){
		listar_marcos();
		matchea = true;
	}

	return matchea;
}

int _parametro_int(char* param){
	return atoi(string_substring_from(param,1));
}

char* _parametro_string(char* param){
	return string_substring_from(param,1);
}

void crear_segment(uint32_t pid, uint32_t tamanio){
	resultado_t* resultado = malloc(sizeof(resultado_t));
	direccion dir = crear_segmento(pid, tamanio, resultado);

	if(*(resultado)==RESULTADO_OK){
		printf("Direccion del segmento creado: %d \n\n", dir);
	}else if(*(resultado)==ERROR_DE_MEMORIA_LLENA){
		printf("No se pudo crear el segmento: ERROR DE MEMORIA LLENA \n\n");
	}

	free(resultado);
	// falta el logueo de la direccion o del error
}

void destruir_segment(uint32_t pid, direccion base){
	resultado_t* resultado = malloc(sizeof(resultado_t));
	destruir_segmento(pid, base, resultado);

	if(*(resultado)==RESULTADO_OK){
		printf("Segmento destruido correctamente \n\n");
	}else if(*(resultado)==ERROR_NO_ENCUENTRO_SEGMENTO){
		printf("ERROR No se pudo encontrar el segmento \n\n");
	}

	free(resultado);
}

void escribir_memo(uint32_t pid, direccion direccion_virtual,
					uint32_t tamanio, char* texto){
	resultado_t* resultado = malloc(sizeof(resultado_t));
	escribir_memoria(pid, direccion_virtual, texto, tamanio, resultado);

	if(*(resultado)==RESULTADO_OK){
		printf("Se escribio la memoria correctamente \n\n");
	}else if(*(resultado)==SEGMENTATION_FAULT){
		printf("ERROR Violacion de segmento \n\n");
	}

	free(resultado);
}

void leer_memo(uint32_t pid, direccion direccion_virtual, uint32_t tamanio){
	resultado_t* resultado = malloc(sizeof(resultado_t));
	char* texto_leido = leer_memoria(pid, direccion_virtual, tamanio, resultado);

	if(*(resultado)==RESULTADO_OK){
		printf("Texto leido: %s \n\n", texto_leido);
	}else if(*(resultado)==SEGMENTATION_FAULT){
		printf("ERROR Violacion de segmento \n\n");
	}

	free(resultado);

	// falta el logueo del texto leido o del error
}

void tabla_segmentos(){
	void _listar_segmentos(proceso_msp_t* proceso) {
		listar_segmentos_de_un_proceso(proceso);
	}
	lock_lista_procesos();
	list_iterate(get_lista_procesos(), (void*) _listar_segmentos);
	unlock_lista_procesos();
}

void tabla_paginas(uint32_t pid){
	void _listar_paginas(segmento_t* segmento) {
		listar_paginas_de_un_segmento(segmento);
	}
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);
	list_iterate(proceso->segmentos, (void*) _listar_paginas);
}

void listar_marcos(){
	void _listar_marcos(marco_t* marco){

		printf("Nro marco: %d \n", marco->id);

		if(marco->ocupado){
			printf("Esta ocupado: %s ", "SI");
			printf("por el proceso con id %d", marco->id_proceso);
		}else{
			printf("Esta ocupado: %s \n", "NO");
		}

	}
	list_iterate(get_lista_marcos(), (void*) _listar_marcos);
}


