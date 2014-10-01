/*
 * mensajes.c
 *
 *  Created on: 24/9/2014
 *      Author: martin
 */

#include "mensajes.h"

flag_t codigo_operacion(char* mensaje)
{
	flag_t cod_op;
	memcpy(&cod_op, mensaje, sizeof(flag_t));

	return cod_op;
}

uint32_t tamanio_flagt()
{
	return sizeof(flag_t);
}
