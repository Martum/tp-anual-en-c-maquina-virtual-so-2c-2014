#include "instrucciones.h"

/*
 * 	Carga en el registro el número dado
 */
resultado_t load(tcb_t* tcb) {
	char registro;
	obtener_registro(tcb, &registro);
	int32_t numero;
	obtener_numero(tcb, &numero);
	return actualizar_valor_en_registro(tcb, registro, numero);
}

/*
 * 	Carga en el primer registro el valor de memoria apuntado por el segundo registro
 */
resultado_t getm(tcb_t* tcb) { // creo que no esta bien
	char registro1;
	obtener_registro(tcb, &registro1);
	char registro2;
	obtener_registro(tcb, &registro2);
	int32_t valor;

	if (obtener_valor_de_registro(tcb, registro2, &valor)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, registro1, valor);
}

/*
 * 	Pone tantos bytes desde el segundo registro hacia la memoria
 * 		apuntada por el primer registro
 */
resultado_t setm(tcb_t* tcb) {
	int32_t numero;
	obtener_numero(tcb, &numero);
	char registro1;
	obtener_registro(tcb, &registro1);
	char registro2;
	obtener_registro(tcb, &registro2);
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

	leer_de_memoria(tcb->pid, direccion1, numero, buffer);
	escribir_en_memoria(tcb->pid, direccion2, numero, buffer);

	free(buffer);

	return OK;
}
/*
 *	Copia el valor del segundo registro hacia el primero
 */
resultado_t movr(tcb_t* tcb) {
	char registro1;
	obtener_registro(tcb, &registro1);
	char registro2;
	obtener_registro(tcb, &registro2);
	int32_t valor;

	if (obtener_valor_de_registro(tcb, registro2, &valor)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, registro1, valor);
}

resultado_t _funcion_operacion(tcb_t* tcb, int32_t operacion(int32_t, int32_t)) {
	char registro1;
	obtener_registro(tcb, &registro1);
	char registro2;
	obtener_registro(tcb, &registro2);
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

/*
 * 	Suma el valor del primer registro con el del segundo registro.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t addr(tcb_t* tcb) {

	int32_t sumar(int32_t valor1, int32_t valor2) {
		return valor1 + valor2;
	}

	return _funcion_operacion(tcb, sumar);
}

/*
 * 	Resta el valor del primer registro con el del segundo registro.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t subr(tcb_t* tcb) {

	int32_t restar(int32_t valor1, int32_t valor2) {
		return valor1 - valor2;
	}

	return _funcion_operacion(tcb, restar);
}

/*
 * 	Multiplica el valor del primer registro con el del segundo registro.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t mulr(tcb_t* tcb) {

	int32_t multiplicar(int32_t valor1, int32_t valor2) {
		return valor1 * valor2;
	}

	return _funcion_operacion(tcb, multiplicar);
}

/*
 * 	Obtiene el resto de la división el valor del primer registro
 * 		con el del segundo registro.
 * 	El resultado de la operación se almacena en elregistro A.
 */
resultado_t modr(tcb_t* tcb) {

	int32_t modulo(int32_t valor1, int32_t valor2) {
		return valor1 % valor2;
	}

	return _funcion_operacion(tcb, modulo);
}

/*
 * 	Divide el valor del primer registro con el del segundo registro.
 * 	El resultado de la operación se almacena en elregistro A.
 */
resultado_t divr(tcb_t* tcb) {

	int32_t dividir(int32_t valor1, int32_t valor2) {
		return valor1 / valor2;
	}

	return _funcion_operacion(tcb, dividir);
}

resultado_t _funcion_incr_decr(tcb_t* tcb, int32_t operacion(int32_t)) {
	char registro;
	obtener_registro(tcb, &registro);
	int32_t valor;

	if (obtener_valor_de_registro(tcb, registro, &valor)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, 'a', operacion(valor));
}

/*
 * 	Incrementa una unidad al valor del registro.
 */
resultado_t incr(tcb_t* tcb) {

	int32_t sumar_1(int32_t valor) {
		return valor++;
	}

	return _funcion_incr_decr(tcb, sumar_1);
}

/*
 * 	Incrementa una unidad al valor del registro.
 */
resultado_t decr(tcb_t* tcb) {

	int32_t restar_1(int32_t valor) {
		return valor--;
	}

	return _funcion_incr_decr(tcb, restar_1);
}

resultado_t _funcion_comparacion(tcb_t* tcb,
		int32_t comparador(int32_t, int32_t)) {
	char registro1;
	obtener_registro(tcb, &registro1);
	char registro2;
	obtener_registro(tcb, &registro2);
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

/*
 * 	Compara si el valor del primer registro es igual al del segundo.
 * 		De ser verdadero, se almacena el valor 1.
 * 		De lo contrario el valor 0.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t comp(tcb_t* tcb) {

	int32_t comparador(int32_t valor1, int32_t valor2) {
		if (valor1 == valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

/*
 * 	Compara si el valor del primer registro es mayor o igual al del segundo.
 * 		De ser verdadero, se almacena el valor 1.
 * 		De lo contrario el valor 0.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t cgeq(tcb_t* tcb) {

	int32_t comparador(int32_t valor1, int32_t valor2) {
		if (valor1 >= valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

/*
 * 	Compara si el valor del primer registro es menor o igual al del segundo.
 * 		De ser verdadero, se almacena el valor 1.
 * 		De lo contrario el valor 0.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t cleq(tcb_t* tcb) {

	int32_t comparador(int32_t valor1, int32_t valor2) {
		if (valor1 <= valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

/*
 * 	Altera el flujo de ejecución para ejecutar la instrucción apuntada por el registro.
 * 	El valor es el desplazamiento desde el inicio del programa.
 */
resultado_t _goto(tcb_t* tcb) {
	char registro;
	obtener_registro(tcb, &registro);
	direccion base_de_codigo = obtener_base_de_codigo(tcb);
	int32_t valor;

	if (obtener_valor_de_registro(tcb, registro, &valor)
			== NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	return actualizar_pc(tcb, base_de_codigo + valor);
}

resultado_t _funcion_de_salto(tcb_t* tcb, int32_t condicion(int32_t)) {
	int32_t offset;
	obtener_numero(tcb, &offset);
	direccion base_de_codigo = obtener_base_de_codigo(tcb);
	int32_t valor;

	if (obtener_valor_de_registro(tcb, 'a', &valor) == NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;
	if (condicion(valor))
		return OK;

	return actualizar_pc(tcb, base_de_codigo + offset);
}

/*
 * 	Altera el flujo de ejecución para ejecutar la instrucción apuntada por el registro,
 * 		solo si el valor del registro A es 0.
 * 	El valor es el desplazamiento desde el inicio del programa.
 */
resultado_t jmpz(tcb_t* tcb) {

	int32_t condicion(int32_t valor) {
		return valor != 0; //TODO es al revez
	}

	return _funcion_de_salto(tcb, condicion);
}

/*
 * 	Altera el flujo de ejecución para ejecutar la instrucción apuntada por el registro,
 * 		solo si el valor del registro A no es 0.
 * 	El valor es el desplazamiento desde el inicio del programa.
 */
resultado_t jpnz(tcb_t* tcb) {

	int32_t condicion(int32_t valor) {
		return valor == 0; //TODO es al revez
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
	int32_t bytes;
	obtener_numero(tcb, &bytes);
	char registro;
	obtener_registro(tcb, &registro);
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
	int32_t bytes;
	obtener_numero(tcb, &bytes);
	char registro;
	obtener_registro(tcb, &registro);
	int32_t valor;
	direccion direccion;
	char buffer[bytes];

	obtener_valor_de_registro(tcb, registro, &valor);
	direccion = (uint32_t) valor;

	leer_de_memoria(tcb->pid, direccion, bytes, buffer);

	escribir_en_memoria(tcb->pid, tcb->cursor_stack, bytes, buffer);

	tcb->cursor_stack = tcb->cursor_stack + bytes;

	return OK;
}

resultado_t take(tcb_t* tcb) {
	int32_t numero;
	obtener_numero(tcb, &numero);
	char registro;
	obtener_registro(tcb, &registro);
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
	if (crear_segmento(tcb->pid, bytes, &direccion)
			== FALLO_CREACION_DE_SEGMENTO)
		return FALLO_CREACION_DE_SEGMENTO;

	return actualizar_valor_en_registro(tcb, 'a', direccion);
}

resultado_t _free(tcb_t* tcb) {
	int32_t valor;

	if (obtener_valor_de_registro(tcb, 'a', &valor) == NO_ENCONTRO_EL_REGISTRO)
		return NO_ENCONTRO_EL_REGISTRO;

	direccion direccion = valor;

	return destruir_segmento(tcb->pid, direccion);
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
