/*
 * direcciones.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */
#include "direcciones.h"
#include <stdlib.h>
#include "proceso_msp.h"
#include "segmento.h"
#include "pagina.h"
#include "swapping.h"


bool memoria_invalida(uint32_t pid, uint32_t direccion_logica)
{
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);
	if(proceso != NULL)
	{
		segmento_t* segmento= buscar_segmento_segun_id_en_lista_segmentos(direccion_logica>>20, proceso->segmentos);
		if(segmento != NULL)
		{
			//Este div devuelve el resto. Esta parte de codigo en general hace lo siguiente:
			//Toma una direccion logica y quita la parte en la que está el id_segmento
			uint16_t id_pagina = div(direccion_logica>>8,0x1000).rem;
			pagina_t* pagina = buscar_pagina_segun_id_en_lista_paginas(id_pagina, segmento->paginas);
			if(pagina != NULL)
			{
				//Si esta en disco, lo traigo a memoria
				if(pagina->en_disco)
				{
					swap_out(proceso->pid, segmento->id,pagina->id);
				}
				//Si la pagina que quiero leer no tiene marco, se lo asigno
				if(!(pagina->tiene_marco))
				{
					asignar_marco(pagina, pid);
				}
				return false;

			}
		}
	}

	return true;
}

bool excede_limite_segmento(uint32_t pid, uint32_t direccion_logica, uint32_t tamanio)
{
	//Desarmo la direccion logica y obtengo el proceso, el segmento, la pagina y el desplazamiento
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);
	segmento_t* segmento= buscar_segmento_segun_id_en_lista_segmentos(direccion_logica>>20, proceso->segmentos);
	uint16_t id_pagina = div(direccion_logica>>8,0x1000).rem;
	uint16_t desplazamiento = div(direccion_logica,0x100).rem;


	//Checkeo que si no entra en una pagina, haya otras paginas donde pueda entrar.
	if((desplazamiento+tamanio)>256)
	{
		uint32_t tamanio_restante= (tamanio+desplazamiento)-256;
		while(tamanio_restante>0)
		{
			if(!hay_siguiente_pagina(id_pagina, segmento->paginas))
			{
				return true;
			}
			tamanio_restante= (tamanio_restante)-4096;

		}

	}
	return false;
}

uint32_t obtener_base_marco(uint32_t pid, direccion direccion_logica)
{
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);

	segmento_t* segmento= buscar_segmento_segun_id_en_lista_segmentos(direccion_logica>>20, proceso->segmentos);

	uint16_t id_pagina = div(direccion_logica>>8,0x1000).rem;
	pagina_t* pagina = buscar_pagina_segun_id_en_lista_paginas(id_pagina, segmento->paginas);

	uint32_t id_marco=pagina->marco;

	return buscar_marco_segun_id(id_marco)->base;
}

