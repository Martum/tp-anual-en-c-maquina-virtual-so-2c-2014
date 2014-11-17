
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <pthread.h>

#include "main.h"
#include "estructuras.h"
#include "configuraciones.h"

#include "consola_msp.h"
#include "conexiones.h"

#include "algoritmos_sustitucion.h"




#include "commons/string.h"
/* EJEMPLO DE LOG
	 * t_log* logger = log_create("logs.txt", "LOG",true, LOG_LEVEL_INFO);
				log_info(logger, "LOG A NIVEL %s", "INFO");
				log_debug(logger, "dale viejio");
	*/

int main(void){
	int pid=1;
	int id_segmento=1;
	int id=1;
	char* nombre_archivo=" ";
		string_append(&nombre_archivo,string_itoa(pid));
		string_append(&nombre_archivo,string_itoa(id_segmento));
		string_append(&nombre_archivo,string_itoa(id));
		printf("%s",nombre_archivo);
	cargar_configuraciones();
	inicializar_lista_procesos();
	inicializar_lista_marcos();
	inicializar_indice_paginas();
	inicializar_memoria_fisica_total();
	inicializar_cantidad_archivos_swap();

	if(strcmp(algoritmo_sustitucion_de_paginas(),"CLOCK")==0){
		setear_puntero_clock();
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
