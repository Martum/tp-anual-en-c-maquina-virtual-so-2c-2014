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
	//ACA PUEDE LLEGAR A ROMPER SI LA FUNCION NO DEVUELVE NULL COMO SE ESPERA QUE HAGA
	//NO SE PUEDE PROBAR EN ESTA INSTANCIA POR ESO SE DEJA A LA SUERTE
	//EN CADA IF PASA LO MISMO

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
					asignar_marco(pagina);
				}
				return false;

			}
		}
	}

	return true;
}

bool excede_limite_segmento(uint32_t direccion_logica, uint32_t tamanio)
{

	return (direccion_logica>>20) < ((direccion_logica+tamanio)>>20);
}

uint32_t obtener_marco(uint32_t pid, direccion direccion_logica)
{
	proceso_msp_t* proceso = buscar_proceso_segun_pid(pid);

	segmento_t* segmento= buscar_segmento_segun_id_en_lista_segmentos(direccion_logica>>20, proceso->segmentos);

	uint16_t id_pagina = div(direccion_logica>>8,0x1000).rem;
	pagina_t* pagina = buscar_pagina_segun_id_en_lista_paginas(id_pagina, segmento->paginas);

	return pagina->marco;
}

