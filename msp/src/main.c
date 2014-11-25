
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <pthread.h>

#include "main.h"
#include "estructuras.h"
#include "configuraciones.h"

#include "consola_msp.h"
#include "conexiones.h"

#include "algoritmos_sustitucion.h"

#include "commons/string.h"

int main(void){
	cargar_configuraciones();
	inicializar_lista_procesos();
	inicializar_lista_marcos();
	inicializar_indice_paginas();
	inicializar_memoria_fisica_total();
	inicializar_cantidad_archivos_swap();

	// Seteo variable necesaria para algoritmo clock
	if(strcmp(algoritmo_sustitucion_de_paginas(),"CLOCK")==0){
		setear_puntero_clock();
	}

	// Creo el directorio en_disco si no existe, alli se guardaran los archivos swap
	struct stat info;
	if( stat("../en_disco", &info ) != 0 ){
		mkdir("../en_disco",0777);
	}

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
