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

typedef struct pagina {
	uint16_t id;
	bool tiene_marco;
	bool en_disco;
	uint32_t marco;
} pagina_t;

#endif /* PAGINA_H_ */
