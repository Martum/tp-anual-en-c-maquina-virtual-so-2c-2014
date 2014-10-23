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
#include "estructuras.h"
#include "marco.h"


void crear_pagina(segmento_t*);

void agregar_pagina_a_segmento(segmento_t*, pagina_t*);

direccion direccion_virtual_segmento_base_pagina(uint16_t id_segmento, uint16_t id_pagina);

pagina_t* buscar_pagina_segun_id_en_lista_paginas(uint16_t id_pagina, t_list* lista_paginas);

void asignar_marco(pagina_t*, uint32_t);

bool hay_siguiente_pagina(uint16_t id_pagina, t_list* lista_paginas);

pagina_t* siguiente_pagina(uint16_t id_pagina, t_list* lista_paginas);


#endif /* PAGINA_H_ */
