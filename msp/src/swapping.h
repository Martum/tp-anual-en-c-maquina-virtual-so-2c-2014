/*
 * swapping.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */


#include "marco.h"
#include "pagina.h"

#ifndef SWAPPING_H_
#define SWAPPING_H_


/*
void swap(pagina_t*);

marco_t* liberar_un_marco();
*/
//Dada una pagina que quiere insertar, busca un marco que liberar y lo inseta ahi
void swap_in(pagina_t*);

//Dada una pagina, la busca en disco y la lleva a memoria
void swap_out(pagina_t*);

//Mediante un algoritmo libera un marco y lo devuelve
marco_t* liberar_un_marco();

//Con un algoritmo predeterminado, toma un marco de la tabla de marcos y lo devuelve para
// ser liberado. No lo libera él mismo.
marco_t* realizar_algoritmo_swapping();

// Mueve el contenido de un marco a disco
void mover_a_disco(marco_t* marco);

#endif /* SWAPPING_H_ */
