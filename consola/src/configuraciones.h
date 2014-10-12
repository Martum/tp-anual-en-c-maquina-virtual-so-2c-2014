/*
 * configuraciones.h
 *
 *  Created on: 12/10/2014
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
 * @RETURNS: La IP donde esta el Kernel
 */
char* ip_kernel();

/**
 * @RETURNS: El puerto donde el Kernel esta escuchando
 */
uint32_t puerto_kernel();



#endif /* CONFIGURACIONES_H_ */
