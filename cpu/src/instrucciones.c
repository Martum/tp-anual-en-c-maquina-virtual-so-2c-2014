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

int32_t getm(tcb_t* tcb) { // creo que no esta bien

	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);
	int32_t valor;

	if (obtener_valor_de_registro_con_puntero(tcb, registro2, &valor) == FALLO)
		return FALLO;

	return actualizar_valor_en_registro(tcb, registro1, valor);
}

int32_t setm(tcb_t* tcb) {
	int32_t numero = obtener_numero(tcb);
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);
	int32_t valor1;
	int32_t valor2;

	if (obtener_valor_de_registro_con_puntero(tcb, registro1, &valor1) == FALLO)
		return FALLO;
	if (obtener_valor_de_registro_con_puntero(tcb, registro2, &valor2) == FALLO)
		return FALLO;

	direccion direccion1 = valor1;
	direccion direccion2 = valor2;

	char* buffer = malloc(numero);

	leer_de_memoria(direccion1, numero, buffer);
	escribir_en_memoria(direccion2, numero, buffer);

	free(buffer);

	return OK;
}

int32_t movr(tcb_t* tcb) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);
	int32_t valor;

	if (obtener_valor_de_registro_con_puntero(tcb, registro2, &valor) == FALLO)
		return FALLO;

	return actualizar_valor_en_registro(tcb, registro1, valor);
}

int32_t _funcion_operacion(tcb_t* tcb, int32_t operacion(int32_t, int32_t)) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);
	int32_t valor1;
	int32_t valor2;
	if (obtener_valor_de_registro_con_puntero(tcb, registro1, &valor1) == FALLO)
		return FALLO;
	if (obtener_valor_de_registro_con_puntero(tcb, registro2, &valor2) == FALLO)
		return FALLO;
	return actualizar_valor_en_registro(tcb, 'a', operacion(valor1, valor2));
}

int32_t addr(tcb_t* tcb) {

	int32_t sumar(int32_t valor1, int32_t valor2) {
		return valor1 + valor2;
	}

	return _funcion_operacion(tcb, sumar);
}

int32_t subr(tcb_t* tcb) {
	int32_t restar(int32_t valor1, int32_t valor2) {
		return valor1 - valor2;
	}

	return _funcion_operacion(tcb, restar);
}

int32_t mulr(tcb_t* tcb) {
	int32_t multiplicar(int32_t valor1, int32_t valor2) {
		return valor1 * valor2;
	}

	return _funcion_operacion(tcb, multiplicar);
}

int32_t modr(tcb_t* tcb) {
	int32_t modulo(int32_t valor1, int32_t valor2) {
		return valor1 % valor2;
	}

	return _funcion_operacion(tcb, modulo);
}

int32_t divr(tcb_t* tcb) {
	int32_t dividir(int32_t valor1, int32_t valor2) {
		return valor1 / valor2;
	}

	return _funcion_operacion(tcb, dividir);
}

int32_t _funcion_incr_decr(tcb_t* tcb, int32_t operacion(int32_t)) {
	char registro = obtener_registro(tcb);
	int32_t valor;
	if (obtener_valor_de_registro_con_puntero(tcb, registro, &valor) == FALLO)
		return FALLO;
	return actualizar_valor_en_registro(tcb, 'a', operacion(valor));
}

int32_t incr(tcb_t* tcb) {
	int32_t sumar_1(int32_t valor) {
		return valor++;
	}

	return _funcion_incr_decr(tcb, sumar_1);
}

int32_t decr(tcb_t* tcb) {
	int32_t restar_1(int32_t valor) {
		return valor--;
	}

	return _funcion_incr_decr(tcb, restar_1);
}

int32_t _funcion_comparacion(tcb_t* tcb, int32_t comparador(int32_t, int32_t)) {
	char registro1 = obtener_registro(tcb);
	char registro2 = obtener_registro(tcb);
	int32_t valor1;
	int32_t valor2;
	if (obtener_valor_de_registro_con_puntero(tcb, registro1, &valor1) == FALLO)
		return FALLO;
	if (obtener_valor_de_registro_con_puntero(tcb, registro2, &valor2) == FALLO)
		return FALLO;
	return actualizar_valor_en_registro(tcb, 'a', comparador(valor1, valor2));
}

int32_t comp(tcb_t* tcb) {
	int32_t comparador(int32_t valor1, int32_t valor2) {
		if (valor1 == valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

int32_t cgeq(tcb_t* tcb) {
	int32_t comparador(int32_t valor1, int32_t valor2) {
		if (valor1 >= valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

int32_t cleq(tcb_t* tcb) {
	int32_t comparador(int32_t valor1, int32_t valor2) {
		if (valor1 <= valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

int32_t _goto(tcb_t* tcb) {
	char registro = obtener_registro(tcb);
	direccion base_de_codigo = obtener_base_de_codigo(tcb);
	int32_t valor;
	if (obtener_valor_de_registro_con_puntero(tcb, registro, &valor) == FALLO)
		return FALLO;
	return actualizar_pc(tcb, base_de_codigo + valor);
}

int32_t jmpz(tcb_t* tcb) {
	int32_t offset = obtener_numero(tcb);
	direccion base_de_codigo = obtener_base_de_codigo(tcb);
	int32_t valor;
	if (obtener_valor_de_registro_con_puntero(tcb, 'a', &valor) == FALLO)
		return FALLO;
	if (valor != 0)
		return OK;
	return actualizar_pc(tcb, base_de_codigo + offset);
}

int32_t jpnz(tcb_t* tcb) {
	int32_t offset = obtener_numero(tcb);
	direccion base_de_codigo = obtener_base_de_codigo(tcb);
	int32_t valor;
	if (obtener_valor_de_registro_con_puntero(tcb, 'a', &valor) == FALLO)
		return FALLO;
	if (valor == 0)
		return OK;
	return actualizar_pc(tcb, base_de_codigo + offset);
}

int32_t inte(tcb_t* tcb) {
	return OK;
}

int32_t flcl(tcb_t* tcb) {
	// fue deprecada
	return OK;
}

int32_t shif(tcb_t* tcb) {
	int32_t bytes = obtener_numero(tcb);
	char registro = obtener_registro(tcb);
	int32_t valor_de_registro;
	if (obtener_valor_de_registro_con_puntero(tcb, 'a',
			&valor_de_registro) == FALLO)
		return FALLO;
	int32_t valor_de_registro_desplazado;
	if (bytes > 0) {
		valor_de_registro_desplazado = valor_de_registro >> bytes;
	} else {
		valor_de_registro_desplazado = valor_de_registro << bytes;
	}
	return actualizar_valor_en_registro(tcb, registro,
			valor_de_registro_desplazado);
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
	return TERMINO;
}

int32_t malc(tcb_t* tcb) {
	int32_t bytes;
	if (obtener_valor_de_registro_con_puntero(tcb, 'a', &bytes) == FALLO)
		return FALLO;
	direccion direccion = crear_segmento(tcb, bytes);
	return actualizar_valor_en_registro(tcb, 'a', direccion);
}

int32_t _free(tcb_t* tcb) {
	int32_t valor;
	if (obtener_valor_de_registro_con_puntero(tcb, 'a', &valor) == FALLO)
		return FALLO;
	direccion direccion = valor;
	return destruir_segmento(tcb, direccion);
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
