/*
 * planificador.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include <hu4sockets/mensajes.h>


void _planificar();

respuesta_de_nuevo_tcb_t  devolver_tcb();

void recibir_tcb(pedido_con_resultado_t);


#endif /* PLANIFICADOR_H_ */
