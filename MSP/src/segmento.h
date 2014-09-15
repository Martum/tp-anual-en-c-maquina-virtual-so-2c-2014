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
#include "pagina.h"
#include "proceso_msp.h"

typedef struct segmento {
	uint16_t id;
	uint32_t tamanio;
	t_list* paginas;
	bool acceso_usuario; /* true si es accesible por un usuario,
						false si es accesible en modo kernel*/

} segmento_t;


segmento_t *crear_segmento(proceso_msp_t*, uint32_t, bool);
void agregar_segmento_a_proceso(segmento_t*, proceso_msp_t*);


#endif /* SEGMENTO_H_ */
