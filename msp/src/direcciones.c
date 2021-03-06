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

bool descomposicion_direccion_logica(uint32_t direccion_logica, uint32_t pid, proceso_msp_t* * proceso, segmento_t* * segmento, pagina_t* * pagina,uint16_t * desplazamiento)
{

	*(proceso) = (buscar_proceso_segun_pid(pid));
	if(*(proceso) != NULL)
	{
		*(segmento)= (buscar_segmento_segun_id_en_lista_segmentos(direccion_logica>>20, (*proceso)->segmentos));
		if(*(segmento) != NULL)
		{
			uint16_t id_pagina = div(direccion_logica>>8,0x1000).rem;
			*(pagina) = (buscar_pagina_segun_id_en_lista_paginas(id_pagina, (*segmento)->paginas));
			if(*(pagina) != NULL)
			{
				//Si esta en disco, lo traigo a memoria
				if((*pagina)->en_disco)
				{
					//TODO No se va a cambiar las cosas si necesito que haga eso
					swap_out((*proceso)->pid, (*segmento)->id,pagina);
				}
				//TODO Realmente debe asignar un marco o deberia tirar un error?
				//Si la pagina que quiero leer no tiene marco, se lo asigno
				if(!((*pagina)->tiene_marco))
				{
					asignar_marco(pagina, segmento, (*proceso)->pid);
				}
				(*desplazamiento) = div(direccion_logica,0x100).rem;
				return false;
			}
		}
	}
	return true;
}


bool excede_limite_segmento(proceso_msp_t* proceso, segmento_t* segmento, pagina_t* pagina, uint16_t desplazamiento, uint32_t tamanio)
{
	uint16_t id_pagina = pagina->id;
	pagina_t * pag_aux=NULL;

	uint16_t pedido_maximo = desplazamiento+tamanio-1;
	uint16_t maximo_byte_disponible = pagina->max_modificable;
	//Si no entra en una pagina, checkeo si hay otras paginas donde pueda entrar.
	if(pedido_maximo > maximo_byte_disponible)
	{
		int tamanio_restante= pedido_maximo - maximo_byte_disponible;
		while(tamanio_restante>0)
		{
			/*Pregunto si hay otra pagina, y si la hay cambio el id por
			 * el de la nueva pagina
			 */
			if(!hay_siguiente_pagina(id_pagina, segmento->paginas, &pag_aux))
			{
				return true;
			}
			id_pagina=pag_aux->id;
			tamanio_restante= (tamanio_restante)-((pagina->max_modificable)+1);
		}

	}

	return false;
}




