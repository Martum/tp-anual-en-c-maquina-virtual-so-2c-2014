/*
 * interfaz.h
 *
 *  Created on: 22/09/2014
 *      Author: utnso
 */

#ifndef INTERFAZ_H_
#define INTERFAZ_H_

#include "estructuras.h"
/* variables globales:
 * 		- tabla de procesos (lista de procesos)
 * 		- cantidad de memoria fisica total pedida
 */


// INTERFAZ DE LA MSP:

/*
 *  crear_segmento(pid, tamanio):direccion_virtual_base
 *
 * 	Crea un segmento para el proceso pid y retorna la direccion virtual base
 * 	del segmento creado.
 *  Si no hay espacio suficiente en la mp o en el espacio de swapping
 *  retorna un error de memoria llena.
 */
direccion crear_segmento(uint32_t, uint32_t);


/*
 * destruir_segmento(pid, base)
 *
 * Destruye el segmento que esta en la direccion virtual base de la tabla de
 * segmentos del proceso pid.
 * Libera la memoria que ocupaba este segmento.
 */
void destruir_segmento(uint32_t pid, direccion base);

/*
 * leer_memoria(pid, direccion_logica, tamanio):bytes_leidos
 *
 * Retorna los bytes comenzando desde la direccon_logica para un proceso pid.
 * La cantidad de bytes a leer sera igual a tamanio.
 * Si la direccion que se pasa es invalida o excede los limites del segmento
 * entonces retorna un error de violacion de segmento.
 */
uint32_t leer_memoria(uint32_t pid, direccion direccon_logica, uint32_t tamanio);

/*
 * escribir_memoria(pid, direccion_logica, bytes_a_escribir, tamanio)
 *
 * Escribe en el proceso pid hasta tamanio bytes del buffer bytes_a_escribir comenzando
 * en la direccion_logica.
 * Si se intenta escribir en una direccion invalida o se exceden los limites del segmento
 * entonces retorna un error de violacion de segmento.
 */
uint32_t escribir_memoria(uint32_t pid, direccion direccon_logica, char* bytes_a_escribir, uint32_t tamanio);

#endif /* INTERFAZ_H_ */
