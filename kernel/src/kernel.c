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

#include "configuraciones.h"
#include "lstestados.h"
#include "loader.h"
#include "conexiones.h"
#include <pthread.h>

#include <hu4sockets/sockets.h>

int jose = 5;

int main(void) {
	printf("- Comienza el Kernel\n");

	// - Levanta las configuraciones del archivo
	cargar_configuraciones();
	printf("- Cargadas las configuraciones\n");

	// - Inicializa las listas del Planificador
	inicializar_listas_estados_tcb();
	printf("- Inicializadas las Listas de estados TCB\n");

	// - Inicailiza las listas de conexiones socketeras
	inicializar_listas_conexiones();
	printf("- Inicializadas las Listas de conexiones\n");


	// - Conectar con la memoria (agregar funcion en conexiones.h)

	// - Crear thread para escuchar_conexiones_entrantes_y_procesos()
	pthread_t conexiones_procesos_thread;
	pthread_create(&conexiones_procesos_thread, NULL, escuchar_conexiones_entrantes_y_procesos, NULL);
	printf("- Creado Thread para escuchar Procesos\n");


	// - Crear threar para escuchar_cpus()
	pthread_t conexiones_cpus_thread;
	pthread_create(&conexiones_cpus_thread, NULL, escuchar_cpus, NULL);
	printf("- Creado Thread para escuchar CPUs\n");

	printf("\nBienvenido al Kernel - FULL-FUNCTIONAL\n");

	// Esperamos a que ambos terminen
	pthread_join(conexiones_procesos_thread, NULL);
	pthread_join(conexiones_cpus_thread, NULL);

	printf("Me fui \\o/");
	return EXIT_SUCCESS;
}
