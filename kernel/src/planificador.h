/*
 * planificador.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include <hu4sockets/mensajes.h>

// RETORNA EL PROXIMO TCB A EJECUTAR.
//respuesta_de_nuevo_tcb_t  proximo_tcb();

// RECIBE UN TCB DEL CPU Y HACE LO QUE CORRESPONDA SEGUN EL RESULTADO.
void recibir_tcb(resultado_t resultado, tcb_t* tcb);


#endif /* PLANIFICADOR_H_ */
