/*
 * instrucciones.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include <commons/collections/dictionary.h>
#include "estructuras.h"

#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

int load(tcb_t* tcb);
int getm(tcb_t* tcb);
int setm(tcb_t* tcb);
int movr(tcb_t* tcb);
int addr(tcb_t* tcb);
int subr(tcb_t* tcb);
int mulr(tcb_t* tcb);
int modr(tcb_t* tcb);
int divr(tcb_t* tcb);
int incr(tcb_t* tcb);
int decr(tcb_t* tcb);
int comp(tcb_t* tcb);
int cgeq(tcb_t* tcb);
int cleq(tcb_t* tcb);
int _goto(tcb_t* tcb);
int jmpz(tcb_t* tcb);
int jpnz(tcb_t* tcb);
int inte(tcb_t* tcb);
int flcl(tcb_t* tcb);
int shif(tcb_t* tcb);
int nopp(tcb_t* tcb);
int push(tcb_t* tcb);
int take(tcb_t* tcb);
int xxxx(tcb_t* tcb);
int malc(tcb_t* tcb);
int _free(tcb_t* tcb);
int innn(tcb_t* tcb);
int innc(tcb_t* tcb);
int outn(tcb_t* tcb);
int outc(tcb_t* tcb);
int crea(tcb_t* tcb);
int join(tcb_t* tcb);
int blok(tcb_t* tcb);
int wake(tcb_t* tcb);
void cargar_diccionario_de_instrucciones(t_dictionary* dic);

#endif /* INSTRUCCIONES_H_ */
