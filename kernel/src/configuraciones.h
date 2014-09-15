/*
 * configuraciones.h
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#ifndef CONFIGURACIONES_H_
#define CONFIGURACIONES_H_

#include <stdint.h>

/**
 * Levanta las configuraciones del archivo
 */
void cargar_configuraciones(void);

/**
 * @NAME: tamanio_stack
 * @DESC: Devuelve TAMANIO_STACK del archivo de Configuraciones
 */
uint32_t tamanio_stack();


#endif /* CONFIGURACIONES_H_ */
