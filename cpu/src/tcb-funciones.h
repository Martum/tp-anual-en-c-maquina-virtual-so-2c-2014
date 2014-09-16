/*
 * tcb-funciones.h
 *
 *  Created on: 16/09/2014
 *      Author: utnso
 */

#ifndef TCB_FUNCIONES_H_
#define TCB_FUNCIONES_H_

void copiar_numero_en_registro(tcb_t* tcb, char* registro, int* numero);
int obtener_valor_de_registro(tcb_t* tcb, char* registro);

#endif /* TCB_FUNCIONES_H_ */
