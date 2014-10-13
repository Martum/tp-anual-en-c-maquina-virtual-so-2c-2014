/*
 * direcciones.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */
#include <stdbool.h>
#include <stdint.h>
#include "estructuras.h"

#ifndef DIRECCIONES_H_
#define DIRECCIONES_H_

//Devuelve true si la memoria es invalida(no existe el proceso, el segmento o la pagina).
//Tambien si la memoria es valida, pero la pagina esta en disco o no tiene marco,
//la lleva a memoria.
bool memoria_invalida(uint32_t pid, uint32_t direccion_logica);

//Checkea si al sumarle el tamanio a la direccion se supera el limite del segmento
bool excede_limite_segmento(uint32_t direccion_logica, uint32_t tamanio);

//A partir del pid y la direccion logica devuelve la base del marco en el que se encuentra
// el dato pedido
uint32_t obtener_base_marco(uint32_t pid, direccion direccion_logica);


#endif /* DIRECCIONES_H_ */
