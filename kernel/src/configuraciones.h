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
 * @RETURNS: El puerto donde se escuchan conexiones
 */
uint32_t puerto();

/**
 * @RETURNS: La IP donde esta la MSP
 */
char* ip_msp();

/**
 * @RETURNS: El puerto donde escucha la MSP
 */
uint32_t puerto_msp();

/**
 * @RETURNS: El Quantum del CPU
 */
uint32_t quantum();

/**
 * @RETURNS: La ruta del archivo de codigo BESO de las Syscalls
 */
char* syscalls();

/**
 * @NAME: tamanio_stack
 * @DESC: Devuelve TAMANIO_STACK del archivo de Configuraciones
 */
uint32_t tamanio_stack();

char* ruta_logeo();


#endif /* CONFIGURACIONES_H_ */
