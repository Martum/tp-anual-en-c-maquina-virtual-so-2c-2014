/*
 * configuraciones.h
 *
 *  Created on: 22/09/2014
 *      Author: utnso
 */

#ifndef CONFIGURACIONES_H_
#define CONFIGURACIONES_H_

t_list* get_lista_procesos();
uint32_t get_memoria_fisica_total();
void inicializar_lista_procesos();
void inicializar_memoria_fisica_total(uint32_t);

char * arma_direccion(uint16_t, int);

#endif /* CONFIGURACIONES_H_ */
