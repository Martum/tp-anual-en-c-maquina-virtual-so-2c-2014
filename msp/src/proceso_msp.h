/*
 * proceso_msp.h
 *
 *  Created on: 14/09/2014
 *      Author: utnso
 */

#ifndef PROCESO_MSP_H_
#define PROCESO_MSP_H_

#include "estructuras.h"

void listar_segmentos_de_un_proceso(proceso_msp_t*);
proceso_msp_t* crear_proceso_msp(uint32_t);
proceso_msp_t* buscar_proceso_segun_pid(uint32_t);
bool quitar_segmento(proceso_msp_t*, direccion);
void _destruye_segmento(segmento_t*);
void _destruye_pagina(pagina_t*);
void _destruye_pagina_de_indice(pagina_t *);

#endif /* PROCESO_MSP_H_ */
