/*
 * segmento.h
 *
 *  Created on: 14/09/2014
 *      Author: utnso
 */

#ifndef SEGMENTO_H_
#define SEGMENTO_H_

#include <commons/collections/list.h>
#include <stdint.h>
#include <stdbool.h>
#include "proceso_msp.h"
#include "estructuras.h"

segmento_t *crear_segmento_con_paginas(proceso_msp_t*, uint32_t);
void agregar_segmento_a_proceso(segmento_t*, proceso_msp_t*);
void listar_paginas(segmento_t*);
uint32_t cantidad_paginas(uint32_t);
direccion get_direccion_virtual(uint32_t,uint32_t,uint16_t);
direccion direccion_virtual_base_de_segmento(uint16_t);

#endif /* SEGMENTO_H_ */
