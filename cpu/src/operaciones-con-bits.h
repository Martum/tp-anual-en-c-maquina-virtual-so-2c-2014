/*
 * operaciones-con-bits.h
 *
 *  Created on: 17/10/2014
 *      Author: utnso
 */

#ifndef OPERACIONES_CON_BITS_H_
#define OPERACIONES_CON_BITS_H_

#include <stdint.h>

/*
 * 	@DESC: Forma un int a partir de un array de cuatro bytes
 * 	@RETURN:
 */
void unir_bytes(int32_t* valor, unsigned char buffer[4]);

/*
 * 	@DESC: Divide un int en cuatro bytes
 * 	@RETURN:
 */
void dividir_en_bytes(int32_t valor, unsigned char bytes[4]);

#endif /* OPERACIONES_CON_BITS_H_ */
