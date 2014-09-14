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

typedef struct segmento {
	uint16_t id;
	t_list* paginas;
	uint32_t tamanio;
} segmento_t;

#endif /* SEGMENTO_H_ */
