#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/dictionary.h>
#include "estructuras.h"
#include "instrucciones.h"
#include "sockets.h"
#include "tcb-funciones.h"
#include "resultados.h"

int load(tcb_t* tcb) {

	char registro = obtener_registro(tcb);
	int numero = obtener_numero(tcb);
	return actualizar_valor_en_registro(tcb, registro, numero);
}

int getm(tcb_t* tcb) {

	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int valor = obtener_valor_de_registro(tcb, registro2);

	int resultado;

	if (valor == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, registro1, valor);
	}

	return resultado;
}

int setm(tcb_t* tcb) {
	int numero = obtener_numero(tcb);
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	direccion direccion1 = obtener_valor_de_registro(tcb, registro1);
	direccion direccion2 = obtener_valor_de_registro(tcb, registro2);

	int resultado = 0;

	char* buffer = malloc(numero);

	leer_de_memoria(direccion1, numero, buffer);
	escribir_en_memoria(direccion2, numero, buffer);

	free(buffer);

	return resultado;
}

int movr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int valor = obtener_valor_de_registro(tcb, registro2);

	int resultado;

	if (valor == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, registro1, valor);
	}

	return resultado;
}

int addr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);

	int resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, 'a',
				valor1 - valor2);
	}

	return resultado;
}

int subr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);

	int resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, 'a',
				valor1 - valor2);
	}

	return resultado;
}

int mulr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);

	int resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, 'a',
				valor1 * valor2);
	}

	return resultado;
}

int modr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);

	int resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, 'a',
				valor1 % valor2);
	}

	return resultado;
}

int divr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);

	int resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else if (valor2 == 0) {
		resultado = actualizar_valor_en_registro(tcb, 'f', ZERO_DIV);
	} else {
		resultado = actualizar_valor_en_registro(tcb, 'a',
				valor1 / valor2);
	}

	return resultado;
}

int incr(tcb_t* tcb) {
	char registro = obtener_registro(tcb);

	int valor = obtener_valor_de_registro(tcb, registro);

	int resultado;

	if (valor == FALLO) {
		resultado = FALLO;
	} else {
		valor++;
		resultado = actualizar_valor_en_registro(tcb, 'a', valor);
	}

	return resultado;
}

int decr(tcb_t* tcb) {
	char registro = obtener_registro(tcb);

	int valor = obtener_valor_de_registro(tcb, registro);

	int resultado;

	if (valor == FALLO) {
		resultado = FALLO;
	} else {
		valor--;
		resultado = actualizar_valor_en_registro(tcb, 'a', valor);
	}

	return resultado;
}

int comp(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	char* registroA = malloc(sizeof(char) + 1);
	*registroA = 'a';

	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);

	int resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else {
		if (valor1 == valor2) {
			resultado = actualizar_valor_en_registro(tcb, 'a', 1);
		} else {
			resultado = actualizar_valor_en_registro(tcb, 'a', 0);
		}
	}

	return resultado;
}

int cgeq(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);

	int resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else {
		if (valor1 >= valor2) {
			resultado = actualizar_valor_en_registro(tcb, 'a', 1);
		} else {
			resultado = actualizar_valor_en_registro(tcb, 'a', 0);
		}
	}

	return resultado;
}

int cleq(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	char* registroA = malloc(sizeof(char) + 1);
	*registroA = 'a';

	int valor1 = obtener_valor_de_registro(tcb, registro1);
	int valor2 = obtener_valor_de_registro(tcb, registro2);

	int resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else {
		if (valor1 <= valor2) {
			resultado = actualizar_valor_en_registro(tcb, 'a', 1);
		} else {
			resultado = actualizar_valor_en_registro(tcb, 'a', 0);
		}
	}

	return resultado;
}

int _goto(tcb_t* tcb) {
	char registro = obtener_registro(tcb);

	int valor = obtener_valor_de_registro(tcb, registro);

	int resultado;

	if (valor == FALLO) {
		resultado = FALLO;
	} else {
		direccion base_de_codigo = obtener_base_de_codigo(tcb);
		resultado = actualizar_pc(tcb, base_de_codigo + valor);
	}

	return resultado;
}

int jmpz(tcb_t* tcb) {
	return OK;
}

int jpnz(tcb_t* tcb) {
	return OK;
}

int inte(tcb_t* tcb) {
	return OK;
}

int flcl(tcb_t* tcb) {
	return OK;
}

int shif(tcb_t* tcb) {
	return OK;
}

int nopp(tcb_t* tcb) {
	return OK;
}

int push(tcb_t* tcb) {
	return OK;
}

int take(tcb_t* tcb) {
	return OK;
}

int xxxx(tcb_t* tcb) {
	return OK;
}

int malc(tcb_t* tcb) {
	return OK;
}

int _free(tcb_t* tcb) {
	return OK;
}

int innn(tcb_t* tcb) {
	return OK;
}

int innc(tcb_t* tcb) {
	return OK;
}

int outn(tcb_t* tcb) {
	return OK;
}

int outc(tcb_t* tcb) {
	return OK;
}

int crea(tcb_t* tcb) {
	return OK;
}

int join(tcb_t* tcb) {
	return OK;
}

int blok(tcb_t* tcb) {
	return OK;
}

int wake(tcb_t* tcb) {
	return OK;
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
