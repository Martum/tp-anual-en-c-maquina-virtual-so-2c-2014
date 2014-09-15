/*
 * proceso_msp.h
 *
 *  Created on: 14/09/2014
 *      Author: utnso
 */

#ifndef PROCESO_MSP_H_
#define PROCESO_MSP_H_

typedef struct proceso_msp{
	uint32_t pid;
	uint32_t tid;
	t_list* segmentos;
}proceso_msp_t;

void listar_segmentos(proceso_msp_t*);
proceso_msp_t* crear_proceso_msp();

#endif /* PROCESO_MSP_H_ */
