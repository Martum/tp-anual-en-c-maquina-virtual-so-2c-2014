/*
 * algoritmos_sustitucion.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#ifndef ALGORITMOS_SUSTITUCION_H_
#define ALGORITMOS_SUSTITUCION_H_

#include <stdlib.h>
#include <stdint.h>
#include "estructuras.h"

// retornan el id del marco elegido para ser desalojado

/* el algoritmo clock requiere que cada vez que una pagina se lee o se escribe
 * su bit_referencia de su lugar en el indice de paginas se setee en 1
 * (USAR FUNCION set_bit_referencia(*pagina))
 */
uint32_t algoritmo_clock(uint16_t * id_pagina_a_swappear);

/* el algoritmo lru requiere que cada vez que una pagina se lee o se escribe
 * se la ubique al principio de la lista indice de paginas
 * (USAR FUNCION ubico_al_principio(*pagina))
 */
uint32_t algoritmo_lru(uint16_t * id_pagina_a_swappear);

void setear_puntero_clock();

void set_bit_referencia(pagina_t*);

void ubico_al_principio(pagina_t*);

void _reasigno_indices(uint64_t);

#endif /* ALGORITMOS_SUSTITUCION_H_ */
