/*
 * configuraciones.h
 *
 *  Created on: 22/09/2014
 *      Author: utnso
 */

#ifndef CONFIGURACIONES_H_
#define CONFIGURACIONES_H_

#include <stdint.h>
#include <commons/collections/list.h>

void cargar_configuraciones();
void destruir_configuraciones();

uint32_t puerto();
uint32_t cantidad_memoria();
uint32_t cantidad_swap();
char* algoritmo_sustitucion_de_paginas();

t_list* get_lista_procesos();
t_list* get_lista_marcos();
t_list* get_indice_paginas();
uint32_t get_memoria_fisica_total();
uint32_t get_cantidad_archivos_swap();

void inicializar_lista_procesos();
void inicializar_lista_marcos();
void inicializar_memoria_fisica_total();
void inicializar_indice_paginas();
void inicializar_cantidad_archivos_swap();

void aumento_cantidad_archivos_swap();
void disminuyo_cantidad_archivos_swap();



#endif /* CONFIGURACIONES_H_ */
