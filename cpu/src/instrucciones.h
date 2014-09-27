/*
 * instrucciones.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdint.h>
#include <commons/collections/dictionary.h>
#include "hu4sockets/tcb.h"

#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

resultado_t load(tcb_t* tcb);
resultado_t getm(tcb_t* tcb);
resultado_t setm(tcb_t* tcb);
resultado_t movr(tcb_t* tcb);
resultado_t addr(tcb_t* tcb);
resultado_t subr(tcb_t* tcb);
resultado_t mulr(tcb_t* tcb);
resultado_t modr(tcb_t* tcb);
resultado_t divr(tcb_t* tcb);
resultado_t incr(tcb_t* tcb);
resultado_t decr(tcb_t* tcb);
resultado_t comp(tcb_t* tcb);
resultado_t cgeq(tcb_t* tcb);
resultado_t cleq(tcb_t* tcb);
resultado_t _goto(tcb_t* tcb);
resultado_t jmpz(tcb_t* tcb);
resultado_t jpnz(tcb_t* tcb);
resultado_t inte(tcb_t* tcb);
resultado_t flcl(tcb_t* tcb);
resultado_t shif(tcb_t* tcb);
resultado_t nopp(tcb_t* tcb);
resultado_t push(tcb_t* tcb);
resultado_t take(tcb_t* tcb);
resultado_t xxxx(tcb_t* tcb);
resultado_t malc(tcb_t* tcb);
resultado_t _free(tcb_t* tcb);
resultado_t innn(tcb_t* tcb);
resultado_t innc(tcb_t* tcb);
resultado_t outn(tcb_t* tcb);
resultado_t outc(tcb_t* tcb);
resultado_t crea(tcb_t* tcb);
resultado_t join(tcb_t* tcb);
resultado_t blok(tcb_t* tcb);
resultado_t wake(tcb_t* tcb);
void cargar_diccionario_de_instrucciones(t_dictionary* dic);

#endif /* INSTRUCCIONES_H_ */
