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
	// - Levanta las configuraciones del archivo
	cargar_configuraciones();

	// - Inicializa las listas del Planificador
	inicializar_listas_estados_tcb();

	// - Inicailiza las listas de conexiones socketeras
	inicializar_listas_conexiones();

	// - Conectar con la memoria (agregar funcion en conexiones.h)

	// - Crear thread para escuchar_conexiones_entrantes_y_procesos()
	pthread_t* conexiones_procesos_thread;
	pthread_create(conexiones_procesos_thread, NULL, escuchar_conexiones_entrantes_y_procesos, NULL);

	// - Crear threar para escuchar_cpus()
	pthread_t* conexiones_cpus_thread;
	pthread_create(conexiones_cpus_thread, NULL, escuchar_cpus, NULL);

	printf("Bienvenido al Kernel");

	// Esperamos a que ambos terminen
	pthread_join(*conexiones_procesos_thread, NULL);
	pthread_join(*conexiones_cpus_thread, NULL);

	return EXIT_SUCCESS;
}
