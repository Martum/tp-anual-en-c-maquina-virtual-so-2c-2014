#include "instrucciones.h"

resultado_t load(tcb_t* tcb) {
	char registro = obtener_registro(*tcb);
	int32_t numero = obtener_numero(*tcb);
	return actualizar_valor_en_registro(tcb, registro, numero);
}

resultado_t getm(tcb_t* tcb) { // creo que no esta bien
	char registro1 = obtener_registro(*tcb);
	char registro2 = obtener_registro(*tcb);
	int32_t valor;

	if (obtener_valor_de_registro(tcb, registro2, &valor)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, registro1, valor);
}

resultado_t setm(tcb_t* tcb) {
	int32_t numero = obtener_numero(*tcb);
	char registro1 = obtener_registro(*tcb);
	char registro2 = obtener_registro(*tcb);
	int32_t valor1;
	int32_t valor2;

	if (obtener_valor_de_registro(tcb, registro1, &valor1)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;
	if (obtener_valor_de_registro(tcb, registro2, &valor2)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	direccion direccion1 = valor1;
	direccion direccion2 = valor2;

	char* buffer = malloc(numero);

	leer_de_memoria(*tcb, direccion1, numero, buffer);
	escribir_en_memoria(*tcb, direccion2, numero, buffer);

	free(buffer);

	return OK;
}

resultado_t movr(tcb_t* tcb) {
	char registro1 = obtener_registro(*tcb);
	char registro2 = obtener_registro(*tcb);
	int32_t valor;

	if (obtener_valor_de_registro(tcb, registro2, &valor)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, registro1, valor);
}

resultado_t _funcion_operacion(tcb_t* tcb, int32_t operacion(int32_t, int32_t)) {
	char registro1 = obtener_registro(*tcb);
	char registro2 = obtener_registro(*tcb);
	int32_t valor1;
	int32_t valor2;

	if (obtener_valor_de_registro(tcb, registro1, &valor1)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;
	if (obtener_valor_de_registro(tcb, registro2, &valor2)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, 'a', operacion(valor1, valor2));
}

resultado_t addr(tcb_t* tcb) {

	int32_t sumar(int32_t valor1, int32_t valor2) {
		return valor1 + valor2;
	}

	return _funcion_operacion(tcb, sumar);
}

resultado_t subr(tcb_t* tcb) {

	int32_t restar(int32_t valor1, int32_t valor2) {
		return valor1 - valor2;
	}

	return _funcion_operacion(tcb, restar);
}

resultado_t mulr(tcb_t* tcb) {

	int32_t multiplicar(int32_t valor1, int32_t valor2) {
		return valor1 * valor2;
	}

	return _funcion_operacion(tcb, multiplicar);
}

resultado_t modr(tcb_t* tcb) {

	int32_t modulo(int32_t valor1, int32_t valor2) {
		return valor1 % valor2;
	}

	return _funcion_operacion(tcb, modulo);
}

resultado_t divr(tcb_t* tcb) {

	int32_t dividir(int32_t valor1, int32_t valor2) {
		return valor1 / valor2;
	}

	return _funcion_operacion(tcb, dividir);
}

resultado_t _funcion_incr_decr(tcb_t* tcb, int32_t operacion(int32_t)) {
	char registro = obtener_registro(*tcb);
	int32_t valor;

	if (obtener_valor_de_registro(tcb, registro, &valor)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, 'a', operacion(valor));
}

resultado_t incr(tcb_t* tcb) {

	int32_t sumar_1(int32_t valor) {
		return valor++;
	}

	return _funcion_incr_decr(tcb, sumar_1);
}

resultado_t decr(tcb_t* tcb) {

	int32_t restar_1(int32_t valor) {
		return valor--;
	}

	return _funcion_incr_decr(tcb, restar_1);
}

resultado_t _funcion_comparacion(tcb_t* tcb,
		int32_t comparador(int32_t, int32_t)) {
	char registro1 = obtener_registro(*tcb);
	char registro2 = obtener_registro(*tcb);
	int32_t valor1;
	int32_t valor2;

	if (obtener_valor_de_registro(tcb, registro1, &valor1)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;
	if (obtener_valor_de_registro(tcb, registro2, &valor2)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, 'a', comparador(valor1, valor2));
}

resultado_t comp(tcb_t* tcb) {

	int32_t comparador(int32_t valor1, int32_t valor2) {
		if (valor1 == valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

resultado_t cgeq(tcb_t* tcb) {

	int32_t comparador(int32_t valor1, int32_t valor2) {
		if (valor1 >= valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

resultado_t cleq(tcb_t* tcb) {

	int32_t comparador(int32_t valor1, int32_t valor2) {
		if (valor1 <= valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

resultado_t _goto(tcb_t* tcb) {
	char registro = obtener_registro(*tcb);
	direccion base_de_codigo = obtener_base_de_codigo(tcb);
	int32_t valor;

	if (obtener_valor_de_registro(tcb, registro, &valor)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	return actualizar_pc(tcb, base_de_codigo + valor);
}

resultado_t _funcion_de_salto(tcb_t* tcb, int32_t condicion(int32_t)) {
	int32_t offset = obtener_numero(*tcb);
	direccion base_de_codigo = obtener_base_de_codigo(tcb);
	int32_t valor;

	if (obtener_valor_de_registro(tcb, 'a', &valor) == NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;
	if (condicion(valor))
		return OK;

	return actualizar_pc(tcb, base_de_codigo + offset);
}

resultado_t jmpz(tcb_t* tcb) {

	int32_t condicion(int32_t valor) {
		return valor != 0;
	}

	return _funcion_de_salto(tcb, condicion);
}

resultado_t jpnz(tcb_t* tcb) {

	int32_t condicion(int32_t valor) {
		return valor == 0;
	}

	return _funcion_de_salto(tcb, condicion);
}

resultado_t inte(tcb_t* tcb) {
	return OK;
}

resultado_t flcl(tcb_t* tcb) {
	// fue deprecada
	return OK;
}

resultado_t shif(tcb_t* tcb) {
	int32_t bytes = obtener_numero(*tcb);
	char registro = obtener_registro(*tcb);
	int32_t valor_de_registro, valor_de_registro_desplazado;

	if (obtener_valor_de_registro(tcb, 'a', &valor_de_registro)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	if (bytes > 0) {
		valor_de_registro_desplazado = valor_de_registro >> bytes;
	} else {
		valor_de_registro_desplazado = valor_de_registro << bytes;
	}

	return actualizar_valor_en_registro(tcb, registro,
			valor_de_registro_desplazado);
}

resultado_t nopp(tcb_t* tcb) {
	return OK;
}

resultado_t push(tcb_t* tcb) {
	return OK;
}

resultado_t take(tcb_t* tcb) {
	return OK;
}

resultado_t xxxx(tcb_t* tcb) {
	return TERMINO;
}

resultado_t malc(tcb_t* tcb) {
	int32_t bytes;

	if (obtener_valor_de_registro(tcb, 'a', &bytes) == NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	direccion direccion;
	if (crear_segmento(*tcb, bytes, &direccion) == FALLO_CREACION_DE_SEGMENTO)
		return FALLO_CREACION_DE_SEGMENTO;

	return actualizar_valor_en_registro(tcb, 'a', direccion);
}

resultado_t _free(tcb_t* tcb) {
	int32_t valor;

	if (obtener_valor_de_registro(tcb, 'a', &valor) == NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	direccion direccion = valor;

	return destruir_segmento(*tcb, direccion);
}

resultado_t innn(tcb_t* tcb) {
	return OK;
}

resultado_t innc(tcb_t* tcb) {
	return OK;
}

resultado_t outn(tcb_t* tcb) {
	return OK;
}

resultado_t outc(tcb_t* tcb) {
	return OK;
}

resultado_t crea(tcb_t* tcb) {
	return OK;
}

resultado_t join(tcb_t* tcb) {
	return OK;
}

resultado_t blok(tcb_t* tcb) {
	return OK;
}

resultado_t wake(tcb_t* tcb) {
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
