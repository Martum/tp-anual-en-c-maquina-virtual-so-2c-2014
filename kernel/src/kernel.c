/*
 ============================================================================
 Name        : kernel.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <commons/config.h>
#include <commons/collections/list.h>
#include "configuraciones.h"
#include "lstestados.h"
#include "loader.h"

int jose = 5;


int main(void) {
	// Levanta las configuraciones del archivo
	//cargar_configuraciones();

	// Inicializa las listas del Planificador
	//inicializar_listas();



	printf("!!!%d!!!", jose); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
