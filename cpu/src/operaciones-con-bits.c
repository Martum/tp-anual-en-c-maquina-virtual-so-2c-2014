/*
 * operaciones-con-bits.c
 *
 *  Created on: 17/10/2014
 *      Author: utnso
 */

#include "operaciones-con-bits.h"

void unir_bytes(int32_t* valor, char buffer[3])
{
	*valor = (unsigned char) buffer[3]
		| ((int32_t) (unsigned char) buffer[2] << 8)
		| ((int32_t) (unsigned char) buffer[1] << 16)
		| ((int32_t) (unsigned char) buffer[0] << 24);

//	unsigned char bytes[3];
//	bytes[0] = buffer[3];
//	bytes[1] = buffer[2];
//	bytes[2] = buffer[1];
//	bytes[3] = buffer[0];
//	*valor = *(int32_t *) bytes;

}

void dividir_en_bytes(int32_t valor, char bytes[3])
{
	/*
	 bytes[0] = (valor >> 24) & 0xFF;
	 bytes[1] = (valor >> 16) & 0xFF;
	 bytes[2] = (valor >> 8) & 0xFF;
	 bytes[3] = valor & 0xFF;
	 */

	bytes[0] = (valor >> 24);
	bytes[1] = (valor >> 16);
	bytes[2] = (valor >> 8);
	bytes[3] = valor;
}

void desplazar_bits(int32_t bits_a_desplazar, int32_t* valor)
{
	if (bits_a_desplazar > 0)
	{
		*valor = *valor >> bits_a_desplazar;
	} else
	{
		*valor = *valor << abs(bits_a_desplazar);
	}
}
