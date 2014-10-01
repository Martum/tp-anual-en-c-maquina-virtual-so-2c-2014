/*
 * algoritmos_sustitucion.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#ifndef ALGORITMOS_SUSTITUCION_H_
#define ALGORITMOS_SUSTITUCION_H_

// retornan el id del marco elegido para ser desalojado

/* el algoritmo clock requiere que cada vez que una pagina se lee o se escribe
 * su bit_referencia de su lugar en el indice de paginas se setee en 1
 */
uint32_t clock();

/* el algoritmo lru requiere que cada vez que una pagina se lee o se escribe
 * se la ubique al principio de la lista indice de paginas
 * (USAR FUNCION ubico_al_principio(*pagina))
 */
uint32_t lru();

void ubico_al_principio(pagina_t*);

#endif /* ALGORITMOS_SUSTITUCION_H_ */
