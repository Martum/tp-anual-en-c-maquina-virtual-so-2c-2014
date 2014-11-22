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

#include "logs.h"

/*
 * @DESC: Carga el archivo de configuraciones
 * @RETURNS:
 * 		OK si pudo cargarlo
 * 		FALLO_CONEXION si hubo problemas al cargarlo
 */
resultado_t cargar_configuraciones();

/*
 * @DESC: Libera el archivo de configuraciones
 * @RETURNS:
 */
void liberar_configuraciones();

/*
 * @DESC: Recupera el valor "IP_KERNEL" de archivo de configuraciones
 * @RETURNS: ip del kernel
 */
char* ip_kernel();

/*
 * @DESC: Recupera el valor "PUERTO_KERNEL" de archivo de configuraciones
 * @RETURNS: puerto del kernel
 */
uint32_t puerto_kernel();

/*
 * @DESC: Recupera el valor "IP_MSP" de archivo de configuraciones
 * @RETURNS: ip del msp
 */
char* ip_msp();

/*
 * @DESC: Recupera el valor "PUERTO_MSP" de archivo de configuraciones
 * @RETURNS: puerto del msp
 */
uint32_t puerto_msp();

/*
 * @DESC: Recupera el valor "RETARDO" de archivo de configuraciones
 * @RETURNS: retardo de las instrucciones
 */
uint32_t retardo();

#endif /* CONFIGURACIONES_H_ */
