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

bool descomposicion_direccion_logica(uint32_t direccion_logica, uint32_t pid, proceso_msp_t *proceso, segmento_t *segmento, pagina_t* pagina,uint16_t desplazamiento)
{

	*(proceso) = *(buscar_proceso_segun_pid(pid));
	if(proceso != NULL)
	{
		*(segmento)= *(buscar_segmento_segun_id_en_lista_segmentos(direccion_logica>>20, proceso->segmentos));
		if(segmento != NULL)
		{
			uint16_t id_pagina = div(direccion_logica>>8,0x1000).rem;
			*(pagina) = *(buscar_pagina_segun_id_en_lista_paginas(id_pagina, segmento->paginas));
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
					asignar_marco(pagina, proceso->pid);
				}
				desplazamiento = div(direccion_logica,0x100).rem;
				return false;
			}
		}
	}
	return true;
}


bool excede_limite_segmento(proceso_msp_t* proceso, segmento_t* segmento, pagina_t* pagina, uint16_t desplazamiento, uint32_t tamanio)
{
	uint16_t id_pagina = pagina->id;

	//Si no entra en una pagina, checkeo si hay otras paginas donde pueda entrar.
	if((desplazamiento+tamanio)>256)
	{
		uint32_t tamanio_restante= (tamanio+desplazamiento)-256;
		while(tamanio_restante>0)
		{
			if(!hay_siguiente_pagina(id_pagina, segmento->paginas))
			{
				return true;
			}
			tamanio_restante= (tamanio_restante)-256;

		}

	}
	return false;
}



