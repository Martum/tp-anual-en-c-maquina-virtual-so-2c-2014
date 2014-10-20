
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

#include "main.h"
#include "estructuras.h"
#include "configuraciones.h"

#include "consola_msp.h"
#include "conexiones.h"

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
	inicializar_lista_conexiones_cpu();

	// Creo hilos
	pthread_t consola_msp_thread;
	pthread_create(&consola_msp_thread, NULL, escuchar_consola_msp, NULL);
	pthread_t conexiones_thread;
	pthread_create(&conexiones_thread, NULL, escuchar_conexiones, NULL);

	// Esperamos a que ambos terminen
	pthread_join(consola_msp_thread, NULL);
	pthread_join(conexiones_thread, NULL);

	destruir_configuraciones();
	return 0;
}
