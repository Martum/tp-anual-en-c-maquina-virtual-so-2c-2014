/*
 * consola_msp.h
 *
 *  Created on: 17/09/2014
 *      Author: utnso
 */

#ifndef CONSOLA_MSP_H_
#define CONSOLA_MSP_H_

#endif /* CONSOLA_MSP_H_ */

// IMPRIMIR = en log y en pantalla

/*
 *  crear_segmento(pid, tamanio)
 *
 * 	Crea un segmento para el proceso pid (usa la funcion de crear_segmento de la interfaz).
 *  Imprime la direccion base del segmento creado.
 */
void crear_segmento(uint32_t, uint32_t);

/*
 * destruir_segmento(pid, base)
 *
 * Destruye el segmento que esta en la direccion virtual base de la tabla de
 * segmentos del proceso pid. (usa la funcion destruir_segmento de la interfaz)
 */
void destruir_segmento(uint32_t, direccion);

/*
 * escribir_memoria(pid, direccion_virtual, tamanio, texto)
 *
 * Escribe en el proceso pid hasta tamanio bytes del texto comenzando
 * en la direccion_virtual.
 * Si se intenta escribir en una direccion invalida o se exceden los limites del segmento
 * entonces retorna un error de violacion de segmento.
 * (usa la funcion escribir_memoria de la interfaz)
 */
void escribir_memoria(uint32_t, direccion, uint32_t, char*);

/*
 * leer_memoria(pid, direccion_virtual, tamanio)
 *
 * Imprime los bytes comenzando desde la direccon_logica para un proceso pid.
 * La cantidad de bytes a leer sera igual a tamanio.
 * (usa la funcion leer_memoria de la interfaz)
 */
void leer_memoria(uint32_t, direccion, uint32_t);

/*
 * tabla_segmentos()
 *
 * Imprime el contenido de las tablas de segmentos de todos los procesos.
 * Por cada segmento imprime: pid, nro segmento, tamanio y direccion base
 * del segmento
 */
void tabla_segmentos();

/*
 * tabla_paginas(pid)
 *
 * Imprime el contenido de las tablas de paginas del proceso pid.
 * Por cada pagina se debe imprime: nro de segmento al que pertenece y
 * si esta en memoria principal o en swap
 */
void tabla_paginas(uint32_t);

/*
 * listar_marcos()
 *
 * Imprime todos los marcos existentes.
 * Por cada marco se imprime: nro de marco, si esta ocupado o no y ṕor qué pid y la
 * info relacionada a a los algorimos de reemplazo de paginas (lru y clock)
 */
void listar_marcos();
