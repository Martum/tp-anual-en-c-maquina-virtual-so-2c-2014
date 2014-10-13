/*
 * planificador.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include <hu4sockets/mensajes.h>

// Retorna el respuesta_de_nuevo_tcb_t* ya serializado.
char*  rta_nuevo_tcb();

// RECIBE UN TCB DEL CPU Y HACE LO QUE CORRESPONDA SEGUN EL RESULTADO.
void recibir_tcb(resultado_t resultado, tcb_t* tcb);


#endif /* PLANIFICADOR_H_ */
