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
#include <pthread.h>

#include <hu4sockets/sockets.h>

int jose = 5;


int main(void) {
	// - Levanta las configuraciones del archivo
	cargar_configuraciones();

	// - Inicializa las listas del Planificador
	inicializar_listas_estados_tcb();

	// - Inicailiza las listas de conexiones socketeras
	inicializar_listas_conexiones();

	// - Conectar con la memoria (agregar funcion en conexiones.h)

	// - Crear thread para escuchar_conexiones_entrantes_y_procesos()

	// - Crear threar para escuchar_cpus()

	printf("Bienvenido al Kernel");
	return EXIT_SUCCESS;
}
