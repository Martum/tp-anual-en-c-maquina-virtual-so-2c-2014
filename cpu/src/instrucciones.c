#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/dictionary.h>
#include "estructuras.h"
#include "instrucciones.h"
#include "sockets.h"

int load(tcb_t* tcb) {

	char* registro = obtener_registro(tcb);
	int* numero = obtener_numero(tcb);

	copiar_numero_en_registro(tcb, registro, numero);

	free(registro);
	free(numero);

	return 0;
}

int getm(tcb_t* tcb) {

	char* registro1 = obtener_registro(tcb);
	char* registro2 = obtener_registro(tcb);

	int* numero = malloc(sizeof(int));
	*numero = obtener_valor_de_registro(tcb, registro2);

	copiar_numero_en_registro(tcb, registro1, numero);

	free(registro1);
	free(registro2);
	free(numero);

	return 0;
}

int setm(tcb_t* tcb) {
	return 0;
}

int movr(tcb_t* tcb) {
	return 0;
}

int addr(tcb_t* tcb) {
	char* registro1 = obtener_registro(tcb);
	char* registro2 = obtener_registro(tcb);
	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);
	tcb->a = valor1 + valor2;
	return 0;
}

int subr(tcb_t* tcb) {
	char* registro1 = obtener_registro(tcb);
	char* registro2 = obtener_registro(tcb);
	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);
	tcb->a = valor1 - valor2;
	return 0;
}

int mulr(tcb_t* tcb) {
	char* registro1 = obtener_registro(tcb);
	char* registro2 = obtener_registro(tcb);
	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);
	tcb->a = valor1 * valor2;
	return 0;
}

int modr(tcb_t* tcb) {
	return 0;
}

int divr(tcb_t* tcb) {
	return 0;
}

int incr(tcb_t* tcb) {
	char* registro = obtener_registro(tcb);
	int valor = obtener_valor_de_registro(tcb, registro);
	valor++;
	copiar_numero_en_registro(tcb, registro, &valor);
	return 0;
}

int decr(tcb_t* tcb) {
	char* registro = obtener_registro(tcb);
	int valor = obtener_valor_de_registro(tcb, registro);
	valor--;
	copiar_numero_en_registro(tcb, registro, &valor);
	free(registro);
	return 0;
}

int comp(tcb_t* tcb) {
	char* registro1 = obtener_registro(tcb);
	char* registro2 = obtener_registro(tcb);
	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);
	if (valor1 == valor2) {
		tcb->a = 1;
	} else {
		tcb->a = 0;
	}
	return 0;
}

int cgeq(tcb_t* tcb) {
	char* registro1 = obtener_registro(tcb);
	char* registro2 = obtener_registro(tcb);
	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);
	if (valor1 >= valor2) {
		tcb->a = 1;
	} else {
		tcb->a = 0;
	}
	return 0;
}

int cleq(tcb_t* tcb) {
	char* registro1 = obtener_registro(tcb);
	char* registro2 = obtener_registro(tcb);
	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);
	if (valor1 <= valor2) {
		tcb->a = 1;
	} else {
		tcb->a = 0;
	}
	return 0;
}

int _goto(tcb_t* tcb) {
	return 0;
}

int jmpz(tcb_t* tcb) {
	return 0;
}

int jpnz(tcb_t* tcb) {
	return 0;
}

int inte(tcb_t* tcb) {
	return 0;
}

int flcl(tcb_t* tcb) {
	return 0;
}

int shif(tcb_t* tcb) {
	return 0;
}

int nopp(tcb_t* tcb) {
	return 0;
}

int push(tcb_t* tcb) {
	return 0;
}

int take(tcb_t* tcb) {
	return 0;
}

int xxxx(tcb_t* tcb) {
	return 0;
}

int malc(tcb_t* tcb) {
	return 0;
}

int _free(tcb_t* tcb) {
	return 0;
}

int innn(tcb_t* tcb) {
	return 0;
}

int innc(tcb_t* tcb) {
	return 0;
}

int outn(tcb_t* tcb) {
	return 0;
}

int outc(tcb_t* tcb) {
	return 0;
}

int crea(tcb_t* tcb) {
	return 0;
}

int join(tcb_t* tcb) {
	return 0;
}

int blok(tcb_t* tcb) {
	return 0;
}

int wake(tcb_t* tcb) {
	return 0;
}

void cargar_diccionario_de_instrucciones(t_dictionary* dic) {
	dictionary_put(dic, "LOAD", load);
	dictionary_put(dic, "GETM", getm);
	dictionary_put(dic, "SETM", setm);
	dictionary_put(dic, "MOVR", movr);
	dictionary_put(dic, "ADDR", addr);
	dictionary_put(dic, "SUBR", subr);
	dictionary_put(dic, "MULR", mulr);
	dictionary_put(dic, "MODR", modr);
	dictionary_put(dic, "DIVR", divr);
	dictionary_put(dic, "INCR", incr);
	dictionary_put(dic, "DECR", decr);
	dictionary_put(dic, "COMP", comp);
	dictionary_put(dic, "CGEQ", cgeq);
	dictionary_put(dic, "CLEQ", cleq);
	dictionary_put(dic, "GOTO", _goto);
	dictionary_put(dic, "JMPZ", jmpz);
	dictionary_put(dic, "JPNZ", jpnz);
	dictionary_put(dic, "INTE", inte);
	dictionary_put(dic, "FLCL", flcl);
	dictionary_put(dic, "SHIF", shif);
	dictionary_put(dic, "NOPP", nopp);
	dictionary_put(dic, "PUSH", push);
	dictionary_put(dic, "TAKE", take);
	dictionary_put(dic, "XXXX", xxxx);
	dictionary_put(dic, "MALC", malc);
	dictionary_put(dic, "FREE", _free);
	dictionary_put(dic, "INNN", innn);
	dictionary_put(dic, "INNC", innc);
	dictionary_put(dic, "OUTN", outn);
	dictionary_put(dic, "OUTC", outc);
	dictionary_put(dic, "CREA", crea);
	dictionary_put(dic, "JOIN", join);
	dictionary_put(dic, "BLOK", blok);
	dictionary_put(dic, "WAKE", wake);
}
