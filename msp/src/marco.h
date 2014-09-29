/*
 * marco.h
 *
 *  Created on: 14/09/2014
 *      Author: utnso
 */

#ifndef MARCO_H_
#define MARCO_H_

#include <stdint.h>
#include <stdbool.h>
#include "estructuras.h"
#include <commons/collections/list.h>

void crear_marco_en_lista_de_marcos(t_list *, uint32_t);
marco_t buscar_marco_segun_id(uint32_t);
marco_t* buscar_marco_segun_id(uint32_t);

#endif /* MARCO_H_ */
