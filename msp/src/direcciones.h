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


//A partir de un pid y una direccion logica, obtiene el proceso, el segmento, la pagina y el
//desplazamiento.
//Devuelve true si la memoria es invalida(no existe el proceso, el segmento o la pagina).
//Tambien si la memoria es valida, pero la pagina esta en disco o no tiene marco,
//la lleva a memoria.
bool descomposicion_direccion_logica(uint32_t direccion_logica, uint32_t pid, proceso_msp_t* proceso, segmento_t* segmento, pagina_t* pagina,uint16_t desplazamiento);

//Checkea si al sumarle el tamanio a la direccion se supera el limite del segmento
bool excede_limite_segmento(proceso_msp_t* proceso, segmento_t* segmento, pagina_t* pagina, uint16_t desplazamiento, uint32_t tamanio);





#endif /* DIRECCIONES_H_ */
