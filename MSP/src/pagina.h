/*
 * pagina.h
 *
 *  Created on: 14/09/2014
 *      Author: utnso
 */

#ifndef PAGINA_H_
#define PAGINA_H_

#include <commons/collections/list.h>
#include <stdint.h>
#include <stdbool.h>
#include "segmento.h"
#include "pagina.h"

typedef struct pagina {
	uint16_t id;
	bool tiene_marco;
	bool en_disco;
	uint32_t marco;
} pagina_t;


void crear_pagina(segmento_t);

void agregar_pagina_a_segmento(segmento_t, pagina_t);

#endif /* PAGINA_H_ */
