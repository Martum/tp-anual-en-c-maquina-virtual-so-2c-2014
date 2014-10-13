/*
 * consola_msp.c
 *
 *  Created on: 17/09/2014
 *      Author: utnso
 */
#include <stdlib.h>
#include "estructuras.h"
#include <stdio.h>

#include "consola_msp.h"
#include "proceso_msp.h"
#include "configuraciones.h"
#include "algoritmos_sustitucion.h"
#include "interfaz.h"
#include "segmento.h"


int main(void){

	/* EJEMPLO DE LOG
	 * t_log* logger = log_create("logs.txt", "LOG",true, LOG_LEVEL_INFO);
				log_info(logger, "LOG A NIVEL %s", "INFO");
				log_debug(logger, "dale viejio");
	*/

	// printf("%d",cantidad_memoria());

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



	// PRUEBA DE ESCRITORIO
/*
	proceso_msp_t* proc = crear_proceso_msp();
	crear_segmento(proc->pid,382);
	crear_segmento(proc->pid,693);

	proceso_msp_t* proc2 = crear_proceso_msp();
	crear_segmento(proc2->pid,256);

	tabla_paginas(proc2->pid);

	listar_marcos();
*/
	/*
	 * ACA VOY A PODER RECIBIR INSTRUCCIONES PARA EJECUTAR LAS FUNCIONES QUE TENGO
	 *
	 * TENGO QUE MATCHEAR CADA LLAMADO CON UNA FUNCION
	 */



	return 0;
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

