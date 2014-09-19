#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <commons/collections/dictionary.h>
#include "estructuras.h"
#include "instrucciones.h"
#include "sockets.h"
#include "tcb-funciones.h"
#include "resultados.h"

int32_t load(tcb_t* tcb) {

	char registro = obtener_registro(tcb);
	int32_t numero = obtener_numero(tcb);
	return actualizar_valor_en_registro(tcb, registro, numero);
}

int32_t getm(tcb_t* tcb) {

	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int32_t valor = obtener_valor_de_registro(tcb, registro2);

	int32_t resultado;

	if (valor == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, registro1, valor);
	}

	return resultado;
}

int32_t setm(tcb_t* tcb) {
	int32_t numero = obtener_numero(tcb);
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	direccion direccion1 = obtener_valor_de_registro(tcb, registro1);
	direccion direccion2 = obtener_valor_de_registro(tcb, registro2);

	int32_t resultado = 0;

	char* buffer = malloc(numero);

	leer_de_memoria(direccion1, numero, buffer);
	escribir_en_memoria(direccion2, numero, buffer);

	free(buffer);

	return resultado;
}

int32_t movr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int32_t valor = obtener_valor_de_registro(tcb, registro2);

	int32_t resultado;

	if (valor == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, registro1, valor);
	}

	return resultado;
}

int32_t addr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int32_t valor1 = obtener_valor_de_registro(tcb, registro1);
	int32_t valor2 = obtener_valor_de_registro(tcb, registro2);

	int32_t resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, 'a',
				valor1 - valor2);
	}

	return resultado;
}

int32_t subr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int32_t valor1 = obtener_valor_de_registro(tcb, registro1);
	int32_t valor2 = obtener_valor_de_registro(tcb, registro2);

	int32_t resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, 'a',
				valor1 - valor2);
	}

	return resultado;
}

int32_t mulr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int32_t valor1 = obtener_valor_de_registro(tcb, registro1);
	int32_t valor2 = obtener_valor_de_registro(tcb, registro2);

	int32_t resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, 'a',
				valor1 * valor2);
	}

	return resultado;
}

int32_t modr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int32_t valor1 = obtener_valor_de_registro(tcb, registro1);
	int32_t valor2 = obtener_valor_de_registro(tcb, registro2);

	int32_t resultado;

	if (valor1 == FALLO || valor2 == FALLO) {
		resultado = FALLO;
	} else {
		resultado = actualizar_valor_en_registro(tcb, 'a',
				valor1 % valor2);
	}

	return resultado;
}

int32_t divr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int32_t valor1 = obtener_valor_de_registro(tcb, registro1);
	int32_t valor2 = obtener_valor_de_registro(tcb, registro2);

	int32_t resultado;

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

int32_t incr(tcb_t* tcb) {
	char registro = obtener_registro(tcb);

	int32_t valor = obtener_valor_de_registro(tcb, registro);

	int32_t resultado;

	if (valor == FALLO) {
		resultado = FALLO;
	} else {
		valor++;
		resultado = actualizar_valor_en_registro(tcb, 'a', valor);
	}

	return resultado;
}

int32_t decr(tcb_t* tcb) {
	char registro = obtener_registro(tcb);

	int32_t valor = obtener_valor_de_registro(tcb, registro);

	int32_t resultado;

	if (valor == FALLO) {
		resultado = FALLO;
	} else {
		valor--;
		resultado = actualizar_valor_en_registro(tcb, 'a', valor);
	}

	return resultado;
}

int32_t comp(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	char* registroA = malloc(sizeof(char) + 1);
	*registroA = 'a';

	int32_t valor1 = obtener_valor_de_registro(tcb, registro1);
	int32_t valor2 = obtener_valor_de_registro(tcb, registro2);

	int32_t resultado;

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

int32_t cgeq(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	int32_t valor1 = obtener_valor_de_registro(tcb, registro1);
	int32_t valor2 = obtener_valor_de_registro(tcb, registro2);

	int32_t resultado;

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

int32_t cleq(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);

	char* registroA = malloc(sizeof(char) + 1);
	*registroA = 'a';

	int32_t valor1 = obtener_valor_de_registro(tcb, registro1);
	int32_t valor2 = obtener_valor_de_registro(tcb, registro2);

	int32_t resultado;

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

int32_t _goto(tcb_t* tcb) {
	char registro = obtener_registro(tcb);

	int32_t valor = obtener_valor_de_registro(tcb, registro);

	int32_t resultado;

	if (valor == FALLO) {
		resultado = FALLO;
	} else {
		direccion base_de_codigo = obtener_base_de_codigo(tcb);
		resultado = actualizar_pc(tcb, base_de_codigo + valor);
	}

	return resultado;
}

int32_t jmpz(tcb_t* tcb) {
	return OK;
}

int32_t jpnz(tcb_t* tcb) {
	return OK;
}

int32_t inte(tcb_t* tcb) {
	return OK;
}

int32_t flcl(tcb_t* tcb) {
	return OK;
}

int32_t shif(tcb_t* tcb) {
	return OK;
}

int32_t nopp(tcb_t* tcb) {
	return OK;
}

int32_t push(tcb_t* tcb) {
	return OK;
}

int32_t take(tcb_t* tcb) {
	return OK;
}

int32_t xxxx(tcb_t* tcb) {
	return OK;
}

int32_t malc(tcb_t* tcb) {
	return OK;
}

int32_t _free(tcb_t* tcb) {
	return OK;
}

int32_t innn(tcb_t* tcb) {
	return OK;
}

int32_t innc(tcb_t* tcb) {
	return OK;
}

int32_t outn(tcb_t* tcb) {
	return OK;
}

int32_t outc(tcb_t* tcb) {
	return OK;
}

int32_t crea(tcb_t* tcb) {
	return OK;
}

int32_t join(tcb_t* tcb) {
	return OK;
}

int32_t blok(tcb_t* tcb) {
	return OK;
}

int32_t wake(tcb_t* tcb) {
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
