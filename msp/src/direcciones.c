/*
 * direcciones.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */
#include "direcciones.h"
#include <stdlib.h>
#include "proceso_msp.h"

bool memoria_invalida(uint32_t pid, uint32_t direccion_logica)
{
	if(buscar_proceso_segun_pid(pid)!= NULL)
	{

	}

	return true;
}

bool excede_limite_segmento(uint32_t direccion_logica, uint32_t tamanio)
{

	return (direccion_logica>>20) < ((direccion_logica+tamanio)>>20);
}

