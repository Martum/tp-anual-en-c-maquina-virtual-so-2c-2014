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
			//pagina_t* pagina =
		}
	}

	return true;
}

bool excede_limite_segmento(uint32_t direccion_logica, uint32_t tamanio)
{

	return (direccion_logica>>20) < ((direccion_logica+tamanio)>>20);
}

