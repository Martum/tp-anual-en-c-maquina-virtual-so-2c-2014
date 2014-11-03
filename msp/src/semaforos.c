/*
 * semaforos.c
 *
 *  Created on: 30/10/2014
 *      Author: utnso
 */

#include <pthread.h>

pthread_mutex_t mutex_lista_procesos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_lista_marcos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_lista_indice_paginas = PTHREAD_MUTEX_INITIALIZER;

void lock_lista_procesos(){
	pthread_mutex_lock(&mutex_lista_procesos);
}

void unlock_lista_procesos(){
	pthread_mutex_unlock(&mutex_lista_procesos);
}

void lock_lista_marcos(){
	pthread_mutex_lock(&mutex_lista_marcos);
}

void unlock_lista_marcos(){
	pthread_mutex_unlock(&mutex_lista_marcos);
}

void lock_lista_indice_paginas(){
	pthread_mutex_lock(&mutex_lista_indice_paginas);
}

void unlock_lista_indice_paginas(){
	pthread_mutex_unlock(&mutex_lista_indice_paginas);
}
