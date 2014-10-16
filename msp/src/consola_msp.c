/*
 * consola_msp.c
 *
 *  Created on: 17/09/2014
 *      Author: utnso
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "estructuras.h"
#include "consola_msp.h"
#include "proceso_msp.h"
#include "configuraciones.h"
#include "algoritmos_sustitucion.h"
#include "interfaz.h"
#include "segmento.h"

#include <commons/string.h>
// #include <commons/log.h>

int main(void){

	/* EJEMPLO DE LOG
	 * t_log* logger = log_create("logs.txt", "LOG",true, LOG_LEVEL_INFO);
				log_info(logger, "LOG A NIVEL %s", "INFO");
				log_debug(logger, "dale viejio");
	*/

	cargar_configuraciones();
	inicializar_lista_procesos();
	inicializar_lista_marcos();
	inicializar_indice_paginas();
	inicializar_memoria_fisica_total();

	printf("Bienvenido a la MSP! \n\n");
	printf("Instrucciones disponibles: \n");
	printf(" - Crear Segmento: pid, tamaño \n");
	printf(" - Destruir Segmento: pid, dir_base \n");
	printf(" - Escribir Memoria: pid, dir_virtual, tamaño, texto \n");
	printf(" - Leer Memoria: pid, dir_virtual, tamaño \n");
	printf(" - Tabla de Segmentos \n");
	printf(" - Tabla de Paginas: pid \n");
	printf(" - Listar Marcos \n\n");
	printf("Esperando instrucciones ... \n");

	_dar_instrucciones();

	destruir_configuraciones();
	return 0;
}

void _dar_instrucciones(){
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

		_dar_instrucciones();
	}
}

int _matcheo_cadena_con_funcion(char *cadena){

	// obtengo el nombre de la funcion
	char** nombre_funcion = string_split(cadena, ":");

// 	printf("%s",nombre_funcion[0]);

	if(strcmp(nombre_funcion[0],"Crear Segmento")==0){
		char** parametros = string_split(nombre_funcion[1], ",");
		crear_segment(atoi(parametros[0]), atoi(parametros[1]));
	}

	if(strcmp(nombre_funcion[0],"Destruir Segmento")==0){

	}

	if(strcmp(nombre_funcion[0],"Escribir Memoria")==0){

	}

	if(strcmp(nombre_funcion[0],"Leer Memoria")==0){

	}

	if(strcmp(nombre_funcion[0],"Tabla de Segmentos")==0){

	}

	if(strcmp(nombre_funcion[0],"Tabla de Paginas")==0){

	}

	if(strcmp(nombre_funcion[0],"Listar Marcos")==0){

	}


	return 1;
}

void crear_segment(uint32_t pid, uint32_t tamanio){
	direccion dir = crear_segmento(pid, tamanio);
	printf("Direccion del segmento creado: %d", dir);
	// falta el logueo de la direccion
}

void destruir_segment(uint32_t pid, direccion base){
	destruir_segmento(pid, base);
}

void escribir_memo(uint32_t pid, direccion direccion_virtual, uint32_t tamanio, char* texto){
	escribir_memoria(pid, direccion_virtual, texto, tamanio);
}

void leer_memo(uint32_t pid, direccion direccion_virtual, uint32_t tamanio){
	char* texto_leido = leer_memoria(pid, direccion_virtual, tamanio);
	printf("Texto leido: %s", texto_leido);
	// falta el logueo del texto leido
}

void tabla_segmentos(){
	void _listar_segmentos(proceso_msp_t* proceso) {
		listar_segmentos_de_un_proceso(proceso);
	}
	list_iterate(get_lista_procesos(), (void*) _listar_segmentos);
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


