/*
 * swapping.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */



#include <stdint.h>
#include "estructuras.h"

#ifndef SWAPPING_H_
#define SWAPPING_H_



//Dada una pagina que quiere insertar, busca un marco que liberar y lo inseta ahi
void swap_in(pagina_t* *, uint32_t, uint32_t);


//Mediante un algoritmo libera un marco y lo devuelve
marco_t* liberar_un_marco();

//Con un algoritmo predeterminado, toma un marco de la tabla de marcos y devuelve la pagina
//que lo contiene para liberarlo. Tambien devuelve el PID y el id_segmento de esa pagina
uint32_t realizar_algoritmo_swapping(uint16_t * id_pagina_swap);

void obtener_segmento_y_pagina(pagina_t* * pagina_swap,segmento_t* * segmento_contenedor, uint16_t id_pag_swap, uint32_t pid);

// Mueve una pagina a disco
void mover_a_disco(pagina_t* * pagina, uint32_t pid,uint16_t id_segmento);

//Dado un pid, id_segmento e id_pagina, busca la pagina en disco y la lleva a memoria
void swap_out(uint32_t pid, uint16_t id_segmento, pagina_t* * id_pagina);


#endif /* SWAPPING_H_ */
