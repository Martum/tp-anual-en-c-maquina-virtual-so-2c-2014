/*
 * instrucciones.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <stdint.h>
#include <commons/collections/dictionary.h>
#include "estructuras.h"

#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

int32_t load(tcb_t* tcb);
int32_t getm(tcb_t* tcb);
int32_t setm(tcb_t* tcb);
int32_t movr(tcb_t* tcb);
int32_t addr(tcb_t* tcb);
int32_t subr(tcb_t* tcb);
int32_t mulr(tcb_t* tcb);
int32_t modr(tcb_t* tcb);
int32_t divr(tcb_t* tcb);
int32_t incr(tcb_t* tcb);
int32_t decr(tcb_t* tcb);
int32_t comp(tcb_t* tcb);
int32_t cgeq(tcb_t* tcb);
int32_t cleq(tcb_t* tcb);
int32_t _goto(tcb_t* tcb);
int32_t jmpz(tcb_t* tcb);
int32_t jpnz(tcb_t* tcb);
int32_t inte(tcb_t* tcb);
int32_t flcl(tcb_t* tcb);
int32_t shif(tcb_t* tcb);
int32_t nopp(tcb_t* tcb);
int32_t push(tcb_t* tcb);
int32_t take(tcb_t* tcb);
int32_t xxxx(tcb_t* tcb);
int32_t malc(tcb_t* tcb);
int32_t _free(tcb_t* tcb);
int32_t innn(tcb_t* tcb);
int32_t innc(tcb_t* tcb);
int32_t outn(tcb_t* tcb);
int32_t outc(tcb_t* tcb);
int32_t crea(tcb_t* tcb);
int32_t join(tcb_t* tcb);
int32_t blok(tcb_t* tcb);
int32_t wake(tcb_t* tcb);
void cargar_diccionario_de_instrucciones(t_dictionary* dic);

#endif /* INSTRUCCIONES_H_ */
