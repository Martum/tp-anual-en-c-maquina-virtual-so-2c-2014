
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

#include "main.h"
#include "estructuras.h"
#include "configuraciones.h"
#include "consola_msp.h"

/* EJEMPLO DE LOG
	 * t_log* logger = log_create("logs.txt", "LOG",true, LOG_LEVEL_INFO);
				log_info(logger, "LOG A NIVEL %s", "INFO");
				log_debug(logger, "dale viejio");
	*/

int main(void){
	cargar_configuraciones();
	inicializar_lista_procesos();
	inicializar_lista_marcos();
	inicializar_indice_paginas();
	inicializar_memoria_fisica_total();

	// Creo hilos
	pthread_t consola_msp_thread;
	pthread_create(&consola_msp_thread, NULL, escuchar_consola_msp, NULL);
	pthread_t conexiones_cpus_thread;
	pthread_create(&conexiones_cpus_thread, NULL, escuchar_cpus, NULL);

	// Esperamos a que ambos terminen
	pthread_join(consola_msp_thread, NULL);
	pthread_join(conexiones_cpus_thread, NULL);

	destruir_configuraciones();
	return 0;
}

void* escuchar_consola_msp(void* otro_ente)
{
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

void* escuchar_cpus(void* otro_ente)
{
	// TODO: SIMILAR A LA SUPERIOR PERO PARA ESCUCHAR CPUS

	return NULL;
}
