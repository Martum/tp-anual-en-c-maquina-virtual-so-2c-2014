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
			printf("Instruccion invalida \n\n");
		}

		_dar_instrucciones_por_consola();
	}
}

bool _matcheo_cadena_con_funcion(char *cadena){

	// la instruccion no puede terminar en ":"
	if(string_ends_with(cadena, ":") || string_ends_with(cadena, ",") || string_starts_with(cadena, ":")){
		return false;
	}

	// obtengo el nombre de la funcion
	char** funcion = string_split(cadena, ":");

 	char* nombre_funcion = funcion[0];
 	char* lista_parametros = funcion[1];

 	if(lista_parametros!= NULL){
 	 	if(string_starts_with(lista_parametros,",")){
 	 		return false;
 	 	}
 	}

 	bool matchea = false;

	if(strcmp(nombre_funcion,"Crear Segmento")==0){

		if(strchr(lista_parametros, ',')==NULL){
			return false;
		}

		char** parametros = string_split(lista_parametros, ",");

		char* param[2];

		param[0] = string_substring_from(parametros[0],1);
		param[1] = string_substring_from(parametros[1],1);

		if(!_hay_parametro(param[0]) ||  !_hay_parametro(param[1])){
			return false;
		}

		crear_segment(atoi(param[0]), atoi(param[1]));
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Destruir Segmento")==0){

		if(strchr(lista_parametros, ',')==NULL){
			return false;
		}

		char** parametros = string_split(lista_parametros, ",");

		char* param[2];

		param[0] = string_substring_from(parametros[0],1);
		param[1] = string_substring_from(parametros[1],1);

		if(!_hay_parametro(param[0]) ||  !_hay_parametro(param[1])){
			return false;
		}

		destruir_segment(atoi(param[0]), atoi(param[1]));
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Escribir Memoria")==0){

		if(strchr(lista_parametros, ',')==NULL){
			return false;
		}

		char** parametros = string_split(lista_parametros, ",");

		char* param[4];
		param[0] = string_substring_from(parametros[0],1);
		param[1] = string_substring_from(parametros[1],1);
		param[2] = string_substring_from(parametros[2],1);
		param[3] = string_substring_from(parametros[3],1);

		if(!_hay_parametro(param[0]) ||  !_hay_parametro(param[1])
				||!_hay_parametro(param[2]) ||  !_hay_parametro(param[3])){
			return false;
		}

		escribir_memo(atoi(param[0]), atoi(param[1]), atoi(param[2]), param[3]);
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Leer Memoria")==0){
		if(strchr(lista_parametros, ',')==NULL){
			return false;
		}
		char** parametros = string_split(lista_parametros, ",");

		char* param[3];
		param[0] = string_substring_from(parametros[0],1);
		param[1] = string_substring_from(parametros[1],1);
		param[2] = string_substring_from(parametros[2],1);

		if(!_hay_parametro(param[0]) ||  !_hay_parametro(param[1])
				||!_hay_parametro(param[2])){
			return false;
		}

		leer_memo(atoi(param[0]), atoi(param[1]), atoi(param[2]));
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Tabla de Segmentos")==0){
		tabla_segmentos();
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Tabla de Paginas")==0){
		char** parametros = string_split(lista_parametros, ",");
		char* param = string_substring_from(parametros[0],1);

		if(!_hay_parametro(param)){
			return false;
		}

		tabla_paginas(atoi(param));
		matchea = true;
	}

	if(strcmp(nombre_funcion,"Listar Marcos")==0){
		listar_marcos();
		matchea = true;
	}

	return matchea;
}

bool _hay_parametro(char* param){
	return (strcmp(param, " ")!=0)
			&& (!string_is_empty(param))
			&& (!string_starts_with(param, " ")
			&&(!string_ends_with(param," ")));
}

void crear_segment(uint32_t pid, uint32_t tamanio){
	resultado_t* resultado = malloc(sizeof(resultado_t));
	direccion dir = crear_segmento(pid, tamanio, resultado);

	if(*(resultado)==RESULTADO_OK){
		printf("Direccion del segmento creado: %x \n\n", dir);
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
	//lock_lista_procesos();
	list_iterate(get_lista_procesos(), (void*) _listar_segmentos);
	//unlock_lista_procesos();
}

void tabla_paginas(uint32_t pid){
	bool _es_proceso(proceso_msp_t* proc) {
		return proc->pid==pid;
	}
	void _listar_paginas(segmento_t* segmento) {
		listar_paginas_de_un_segmento(segmento);
	}

	if(list_any_satisfy(get_lista_procesos(),(void*) _es_proceso)){
		proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);
		list_iterate(proceso->segmentos, (void*) _listar_paginas);
	}else{
		printf("ERROR No se pudo encontrar el proceso con pid: %d \n\n", pid);
	}

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
	//lock_lista_marcos();
	list_iterate(get_lista_marcos(), (void*) _listar_marcos);
	//unlock_lista_marcos();
}


