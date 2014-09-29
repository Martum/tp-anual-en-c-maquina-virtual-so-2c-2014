/*
 * configuraciones.h
 *
 *  Created on: 22/09/2014
 *      Author: utnso
 */

#ifndef CONFIGURACIONES_H_
#define CONFIGURACIONES_H_

void cargar_configuraciones();

uint32_t puerto();
uint32_t cantidad_memoria();
uint32_t cantidad_swap();
char* algoritmo_sustitucion_de_paginas();
t_list* get_lista_procesos();
uint32_t get_memoria_fisica_total();
void inicializar_lista_procesos();
void inicializar_memoria_fisica_total(uint32_t);

//char * arma_direccion(uint16_t, int);

#endif /* CONFIGURACIONES_H_ */
