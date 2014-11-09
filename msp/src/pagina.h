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

//Crea una pagina a partir de un segmento y la incluye en la tabla de paginas del segmento
void crear_pagina(segmento_t*);

//Incluye en la tabla de paginas del segmento
void agregar_pagina_a_segmento(segmento_t*, pagina_t*);

//Devuelve la direccion virtual BASE con el segmento y la pagina(desplazamiento=0)
direccion direccion_virtual_segmento_base_pagina(uint16_t id_segmento, uint16_t id_pagina);

//Busca la pagina en la lista de paginas
pagina_t* buscar_pagina_segun_id_en_lista_paginas(uint16_t id_pagina, t_list* lista_paginas);

//Le da a una pagina un espacio en memoria(el pid es necesario)
void asignar_marco(pagina_t* * pagina, uint32_t pid);

//Busca en la tabla de paginas si la pagina pasada por parametro no es la ultima pagina
bool hay_siguiente_pagina(uint16_t * id_pagina, t_list* lista_paginas);

//Devuelve la pagina siguiente al id pasado por parametro, dentro de la lista_paginas
pagina_t* siguiente_pagina(uint16_t id_pagina, t_list* lista_paginas);


#endif /* PAGINA_H_ */
