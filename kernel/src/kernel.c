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

int jose = 5;

t_list* ready;
t_list* block;
t_list* exec;



int main(void) {
	// Levanta las configuraciones del archivo
	cargar_configuraciones();


	printf("!!!%d!!!", jose); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
