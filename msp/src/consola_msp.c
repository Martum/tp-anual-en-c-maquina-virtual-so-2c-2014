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
#include "configuraciones.h"
#include "algoritmos_sustitucion.h"
#include "interfaz.h"


int main(void){
	cargar_configuraciones();
	inicializar_lista_procesos();
	inicializar_lista_marcos();
	inicializar_indice_paginas();
	inicializar_memoria_fisica_total();

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

}

void tabla_paginas(uint32_t pid){

}

void listar_marcos(){

}

