/*
 * swapping.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */


#include "marco.h"
#include "pagina.h"

#ifndef SWAPPING_H_
#define SWAPPING_H_

//Dada una pagina que quiere insertar, busca un marco que liberar y lo inseta ahi
void swap_in(pagina_t*);

//Mediante un algoritmo libera un marco y lo devuelve
marco_t* liberar_un_marco();

//Con un algoritmo predeterminado, toma un marco de la tabla de marcos y lo devuelve para
// ser liberado. Además asigna valores a las variables globales pid, id_segmento, id_pagina
marco_t* realizar_algoritmo_swapping(uint32_t, uint16_t, uint16_t);

// Mueve el contenido de un marco a disco
void mover_a_disco(marco_t* marco);

#endif /* SWAPPING_H_ */
