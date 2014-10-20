/*
 * configuraciones.h
 *
 *  Created on: 22/09/2014
 *      Author: utnso
 */

#ifndef CONFIGURACIONES_H_
#define CONFIGURACIONES_H_

#include <stdint.h>
#include <commons/config.h>
#include <hu4sockets/resultados.h>

resultado_t cargar_configuraciones();
void destruir_configuraciones();

char* ip_kernel();
char* ip_msp();
uint32_t puerto_kernel();
uint32_t puerto_msp();
uint32_t retardo();


#endif /* CONFIGURACIONES_H_ */
