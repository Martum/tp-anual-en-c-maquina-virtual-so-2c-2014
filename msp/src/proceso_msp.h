/*
 * proceso_msp.h
 *
 *  Created on: 14/09/2014
 *      Author: utnso
 */

#ifndef PROCESO_MSP_H_
#define PROCESO_MSP_H_

#include "estructuras.h"

void listar_segmentos(proceso_msp_t*);
proceso_msp_t* crear_proceso_msp();
proceso_msp_t* buscar_proceso_segun_pid(uint32_t pid);

#endif /* PROCESO_MSP_H_ */
