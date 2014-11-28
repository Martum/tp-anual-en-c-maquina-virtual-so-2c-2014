/*
 * marco.h
 *
 *  Created on: 14/09/2014
 *      Author: utnso
 */

#ifndef MARCO_H_
#define MARCO_H_

#include <stdint.h>
#include <stdbool.h>
#include "estructuras.h"
#include <commons/collections/list.h>

//Crea un nuevo marco(y lo inicializa) en la tabla de marcos
void crear_marco_en_lista_de_marcos(t_list * marcos);

//Busca el marco segun su ID en la tabla de marcos
marco_t* buscar_marco_segun_id(uint32_t marco);

//Busca un marco que no este asignado a una pagina
marco_t* buscar_marco_libre();

//Devuelve la cantidad de marcos libres.
uint32_t cantidad_marcos_libre();

//GUARDA QUE PUEDE LLEGAR A TENER ALTOS BUGS
//Toma los datos que hay en el marco y los inserta despues del desplzamiento.
//Modifica tamanio para saber claramente cuanto tamaño queda leer.
char* leer_marco(char* datos_marco, uint16_t desplazamiento, uint32_t * tamanio, int * mas_paginas, uint8_t max_modificable);

//GUARDA QUE PUEDE LLEGAR A TENER ALTOS BUGS
//Toma los bytes_a_escribir y los inserta en datos_marco a partir del desplazamiento.
//Modifica tamanio para saber claramente cuanto tamaño queda escribir.
void escribir_marco(marco_t* * datos_marco, uint16_t desplazamiento, uint32_t * tamanio, char* * bytes_a_escribir, int * mas_paginas, uint8_t max_modificable);

//Funcion auxiliar. Calcula el tamanio que realmente voy a usar, no el total.
uint32_t _calcular_tamanio_real(uint32_t * tamanio, uint16_t desplazamiento, int * mas_paginas);

void liberar_marcos_proceso(uint32_t pid);
#endif /* MARCO_H_ */
