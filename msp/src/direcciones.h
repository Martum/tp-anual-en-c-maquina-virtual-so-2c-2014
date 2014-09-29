/*
 * direcciones.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */
#include <stdbool.h>
#include <stdint.h>

#ifndef DIRECCIONES_H_
#define DIRECCIONES_H_

bool memoria_invalida(uint32_t pid, uint32_t direccion_logica);
bool excede_limite_segmento(uint32_t direccion_logica, uint32_t tamanio);


#endif /* DIRECCIONES_H_ */
