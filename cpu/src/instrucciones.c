// TODO preguntar si cpu puede escribir en memoria
// TODO preguntar si cpu puede crear segmentos

#include "instrucciones.h"
// TODO preguntar si cpu puede destruir segmentos

t_dictionary* dic_instrucciones;

/*
 * 	LOAD [Registro], [Numero]
 *
 * 	Carga en el registro el número dado
 */
resultado_t load(tcb_t* tcb)
{
	char registro;
	int32_t numero;

	if (leer_registro(tcb, &registro) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (leer_numero(tcb, &numero) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (actualizar_valor_del_registro(tcb, registro, numero)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	return OK;
}

/*
 * 	GETM [Registro], [Registro]
 *
 * 	Carga en el primer registro el valor de memoria apuntado por el segundo registro
 */
resultado_t getm(tcb_t* tcb)
{
	char registro1, registro2;

	if (leer_registro(tcb, &registro1) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (leer_registro(tcb, &registro2) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	int32_t valor_del_registro_2;

	if (obtener_valor_del_registro(tcb, registro2, &valor_del_registro_2)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	char buffer;

	if (leer_de_memoria(tcb->pid, valor_del_registro_2, 1, &buffer)
		== FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	int32_t valor_de_memoria = buffer;

	if (actualizar_valor_del_registro(tcb, registro1, valor_de_memoria)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	return OK;
}

/*
 * 	@DESC: 	Escribe en memoria en direccion hacia tantos bytes como cantidad_de_bytes.
 * 			Los bytes los lee de la direccion desde.
 */
resultado_t _copiar_valores(int32_t cantidad_de_bytes, direccion hacia,
	direccion desde, tcb_t* tcb)
{
	char* buffer = malloc(cantidad_de_bytes);

	if (leer_de_memoria(tcb->pid, desde, cantidad_de_bytes, buffer)
		== FALLO_LECTURA_DE_MEMORIA)
	{
		free(buffer);
		return ERROR_EN_EJECUCION;
	}

	if (escribir_en_memoria(tcb->pid, hacia, cantidad_de_bytes, buffer)
		== FALLO_ESCRITURA_EN_MEMORIA)
	{
		free(buffer);
		return ERROR_EN_EJECUCION;
	}

	return OK;
}

/*
 * 	SETM [Numero], [Registro], [Registro]
 *
 * 	Pone tantos bytes desde el segundo registro hacia la memoria
 * 		apuntada por el primer registro
 */
resultado_t setm(tcb_t* tcb)
{
	int32_t cantidad_de_bytes_a_copiar;
	char registro1, registro2;

	if (leer_numero(tcb, &cantidad_de_bytes_a_copiar)
		== FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (leer_registro(tcb, &registro1) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (leer_registro(tcb, &registro2) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	int32_t valor_del_registro_1, valor_del_registro_2;

	if (obtener_valor_del_registro(tcb, registro1, &valor_del_registro_1)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	if (obtener_valor_del_registro(tcb, registro2, &valor_del_registro_2)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	direccion hacia = valor_del_registro_1;
	direccion desde = valor_del_registro_2;

	return _copiar_valores(cantidad_de_bytes_a_copiar, hacia, desde, tcb);
}

/*
 * 	MOVR [Registro], [Registro]
 *
 *	Copia el valor del segundo registro hacia el primero
 */
resultado_t movr(tcb_t* tcb)
{
	char registro1, registro2;

	if (leer_registro(tcb, &registro1) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (leer_registro(tcb, &registro2) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	int32_t valor_del_registro_2;

	if (obtener_valor_del_registro(tcb, registro2, &valor_del_registro_2)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	if (actualizar_valor_del_registro(tcb, registro1, valor_del_registro_2)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	return OK;
}

/*
 * 	@DESC: 	Lee dos bytes desde el pc del tcb, efectua operacion con ambos y los guarda en registro 'a'.
 * 			Cada byte es una letra de un registro.
 * 			Busca el valor del registro.
 */
resultado_t _funcion_operacion(tcb_t* tcb, int32_t operacion(int32_t, int32_t),
	int32_t condicion(int32_t))
{
	char registro1, registro2;

	if (leer_registro(tcb, &registro1) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (leer_registro(tcb, &registro2) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	int32_t valor_del_registro_1, valor_del_registro_2;

	if (obtener_valor_del_registro(tcb, registro1, &valor_del_registro_1)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	if (obtener_valor_del_registro(tcb, registro2, &valor_del_registro_2)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	if (condicion(valor_del_registro_2))
		return ERROR_EN_EJECUCION;

	actualizar_registro_a(tcb,
		operacion(valor_del_registro_1, valor_del_registro_2));

	return OK;
}

/*
 * 	ADDR [Registro], [Registro]
 *
 * 	Suma el valor del primer registro con el del segundo registro.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t addr(tcb_t* tcb)
{

	int32_t sumar(int32_t valor1, int32_t valor2)
	{
		return valor1 + valor2;
	}

	int32_t condicion(int32_t valor1)
	{
		return false;
	}

	return _funcion_operacion(tcb, sumar, condicion);
}

/*
 * 	SUBR [Registro], [Registro]
 *
 * 	Resta el valor del primer registro con el del segundo registro.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t subr(tcb_t* tcb)
{

	int32_t restar(int32_t valor1, int32_t valor2)
	{
		return valor1 - valor2;
	}

	int32_t condicion(int32_t valor1)
	{
		return false;
	}

	return _funcion_operacion(tcb, restar, condicion);
}

/*
 * 	MULR [Registro], [Registro]
 *
 * 	Multiplica el valor del primer registro con el del segundo registro.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t mulr(tcb_t* tcb)
{

	int32_t multiplicar(int32_t valor1, int32_t valor2)
	{
		return valor1 * valor2;
	}

	int32_t condicion(int32_t valor1)
	{
		return false;
	}

	return _funcion_operacion(tcb, multiplicar, condicion);
}

/*
 * 	MODR [Registro], [Registro]
 *
 * 	Obtiene el resto de la división el valor del primer registro
 * 		con el del segundo registro.
 * 	El resultado de la operación se almacena en elregistro A.
 */
resultado_t modr(tcb_t* tcb)
{

	int32_t modulo(int32_t valor1, int32_t valor2)
	{
		return valor1 % valor2;
	}

	int32_t condicion(int32_t valor1)
	{
		return false;
	}

	return _funcion_operacion(tcb, modulo, condicion);
}

/*
 * 	DIVR [Registro], [Registro]
 *
 * 	Divide el valor del primer registro con el del segundo registro.
 * 	El resultado de la operación se almacena en elregistro A.
 */
resultado_t divr(tcb_t* tcb)
{

	int32_t division(int32_t valor1, int32_t valor2)
	{
		return valor1 / valor2;
	}

	int32_t condicion(int32_t valor)
	{
		return valor == 0;
	}

	return _funcion_operacion(tcb, division, condicion);
}

/*
 * 	@DESC: 	Lee un bytes desde el pc del tcb, efectua operacion con el valor y lo guarda en registro 'a'.
 * 			El byte es una letra de un registro.
 * 			Busca el valor del registro.
 */
resultado_t _funcion_incr_decr(tcb_t* tcb, int32_t operacion(int32_t))
{
	char registro;

	if (leer_registro(tcb, &registro) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	int32_t valor_del_registro;

	if (obtener_valor_del_registro(tcb, registro, &valor_del_registro)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	actualizar_registro_a(tcb, operacion(valor_del_registro));

	return OK;
}

/*
 * 	INCR [Registro]
 *
 * 	Incrementa una unidad al valor del registro.
 */
resultado_t incr(tcb_t* tcb)
{

	int32_t sumar_1(int32_t valor)
	{
		return valor++;
	}

	return _funcion_incr_decr(tcb, sumar_1);
}

/*
 * 	DECR [Registro]
 *
 * 	Decrementa una unidad al valor del registro.
 */
resultado_t decr(tcb_t* tcb)
{

	int32_t restar_1(int32_t valor)
	{
		return valor--;
	}

	return _funcion_incr_decr(tcb, restar_1);
}

/*
 * 	@DESC:	Lee dos bytes desde el pc del tcb, efectua comparacion con ambos y los guarda en registro 'a'.
 * 	Cada byte es una letra de un registro.
 * 	Busca el valor del registro.
 */
resultado_t _funcion_comparacion(tcb_t* tcb,
	int32_t comparador(int32_t, int32_t))
{
	char registro1, registro2;

	if (leer_registro(tcb, &registro1) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (leer_registro(tcb, &registro2) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	int32_t valor_del_registro_1, valor_del_registro_2;

	if (obtener_valor_del_registro(tcb, registro1, &valor_del_registro_1)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	if (obtener_valor_del_registro(tcb, registro2, &valor_del_registro_2)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	actualizar_registro_a(tcb,
		comparador(valor_del_registro_1, valor_del_registro_2));

	return OK;
}

/*
 * 	COMP [Registro], [Registro]
 *
 * 	Compara si el valor del primer registro es igual al del segundo.
 * 		De ser verdadero, se almacena el valor 1.
 * 		De lo contrario el valor 0.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t comp(tcb_t* tcb)
{

	int32_t comparador(int32_t valor1, int32_t valor2)
	{
		if (valor1 == valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

/*
 * 	CGEQ [Registro], [Registro]
 *
 * 	Compara si el valor del primer registro es mayor o igual al del segundo.
 * 		De ser verdadero, se almacena el valor 1.
 * 		De lo contrario el valor 0.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t cgeq(tcb_t* tcb)
{

	int32_t comparador(int32_t valor1, int32_t valor2)
	{
		if (valor1 >= valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

/*
 * 	CLEQ [Registro], [Registro]
 *
 * 	Compara si el valor del primer registro es menor o igual al del segundo.
 * 		De ser verdadero, se almacena el valor 1.
 * 		De lo contrario el valor 0.
 * 	El resultado de la operación se almacena en el registro A.
 */
resultado_t cleq(tcb_t* tcb)
{

	int32_t comparador(int32_t valor1, int32_t valor2)
	{
		if (valor1 <= valor2)
			return 1;
		return 0;
	}

	return _funcion_comparacion(tcb, comparador);
}

/*
 * 	GOTO [Registro]
 *
 * 	Altera el flujo de ejecución para ejecutar la instrucción apuntada por el registro.
 * 	El valor es el desplazamiento desde el inicio del programa.
 */
resultado_t _goto(tcb_t* tcb)
{
	char registro;

	if (leer_registro(tcb, &registro) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	int32_t valor_del_registro;

	if (obtener_valor_del_registro(tcb, registro, &valor_del_registro)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	direccion base_de_codigo = obtener_base_de_codigo(tcb);

	actualizar_pc(tcb, base_de_codigo + valor_del_registro);

	return OK;
}

/*
 * 	@DESC:	Lee 4 bytes desde el pc del tcb. Si cumple la condicion, actualiza el valor del pc.
 * 			Los cuatro bytes conforman un numero.
 * 			Efectua condicion el valor del registro 'a'.
 * 			Actualiza el valor del pc con base de codigo + numero leido.
 */
resultado_t _funcion_de_salto(tcb_t* tcb, int32_t condicion(int32_t))
{
	int32_t offset;

	leer_numero(tcb, &offset);

	int32_t valor_del_registro_a = obtener_valor_registro_a(tcb);

	if (condicion(valor_del_registro_a))
		return OK;

	direccion base_de_codigo = obtener_base_de_codigo(tcb);

	actualizar_pc(tcb, base_de_codigo + offset);

	return OK;
}

/*
 * 	JMPZ [Numero]
 *
 * 	Altera el flujo de ejecución para ejecutar la instrucción apuntada por el registro,
 * 		solo si el valor del registro A es 0.
 * 	El valor es el desplazamiento desde el inicio del programa.
 */
resultado_t jmpz(tcb_t* tcb)
{

	int32_t condicion(int32_t valor)
	{
		return valor != 0;
	}

	return _funcion_de_salto(tcb, condicion);
}

/*
 * 	JPNZ [Numero]
 *
 * 	Altera el flujo de ejecución para ejecutar la instrucción apuntada por el registro,
 * 		solo si el valor del registro A no es 0.
 * 	El valor es el desplazamiento desde el inicio del programa.
 */
resultado_t jpnz(tcb_t* tcb)
{

	int32_t condicion(int32_t valor)
	{
		return valor == 0;
	}

	return _funcion_de_salto(tcb, condicion);
}

/*
 * 	INTE [Direccion]
 *
 * 	Interrumpe la ejecución del programa para ejecutar la rutina del kernel
 * 		que se encuentra en la posición apuntada por la direccion.
 * 	El ensamblador admite ingresar una cadena indicando el nombre,
 * 		que luego transformará en el número correspondiente.
 * 	Los posibles valores son
 * 		“MALC”, “FREE”,	“INNN”,	“INNC”, “OUTN”,
 * 		“OUTC”, “BLOK”, “WAKE”, “CREA”, “JOIN”.
 * 	Invoca al servicio correspondiente en el proceso Kernel.
 * 	Notar que el hilo en cuestión debe bloquearse tras una interrupción.
 */
resultado_t inte(tcb_t* tcb)
{
	return EXCEPCION_POR_INTERRUPCION;
}

/*
 * 	SHIF [Número], [Registro]
 *
 * 	Desplaza los bits del registro, tantas veces como se indique en el número.
 * 		De ser desplazamiento positivo, se considera hacia la derecha.
 * 		De lo contrario hacia la izquierda.
 */
resultado_t shif(tcb_t* tcb)
{
	int32_t bits_a_desplazar;
	char registro;

	if (leer_numero(tcb, &bits_a_desplazar) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (leer_registro(tcb, &registro) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	int32_t valor_de_registro;

	if (obtener_valor_del_registro(tcb, registro, &valor_de_registro)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	desplazar_bits(bits_a_desplazar, &valor_de_registro);

	actualizar_valor_del_registro(tcb, registro, valor_de_registro);

	return OK;
}

/*
 * 	NOPP
 *
 * 	Consume un ciclo del CPU sin hacer nada
 */
resultado_t nopp(tcb_t* tcb)
{
	return OK;
}

// TODO preguntar si cpu puede escribir en memoria
/*
 * 	@DESC: Agrega los bytes al stack del tcb y actualiza el cursor del stack.
 */
resultado_t _push(tcb_t* tcb, int32_t cantidad_de_bytes, char bytes[4])
{
	if (escribir_en_memoria(tcb->pid, tcb->cursor_stack, cantidad_de_bytes,
		bytes) == FALLO_ESCRITURA_EN_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (mover_cursor_stack(tcb, cantidad_de_bytes)
		== EXCEPCION_POR_POSICION_DE_STACK_INVALIDA)
		return ERROR_EN_EJECUCION;

	return OK;
}

/*
 *  PUSH [Número], [Registro]
 *
 * 	Apila los primeros bytes, indicado por el número, del valor del registro hacia el stack.
 * 	Modifica el valor del registro cursor de stack de forma acorde.
 */
resultado_t push(tcb_t* tcb)
{
	/*
	 CASO DE USO:

	 El valor del registro es un int, por ejemplo
	 0x072A4704 	(expresado en hexa)
	 120211204 	(expresado en decimal)
	 00000111 00101010 01000111 00000100 (expresado en binario)

	 El primer byte es 07, 7, 00000111
	 El segundo byte es 2A, 42, 00101010
	 El tercer byte es 47, 71, 01000111
	 El cuarto byte es 04, 4, 00000100

	 El numero de bytes a guardar es 2

	 Lo que va a hacer push es guardar en la pila primero 07 y despues 2A

	 */

	int32_t cantidad_de_bytes;
	char registro;

	if (leer_numero(tcb, &cantidad_de_bytes) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (leer_registro(tcb, &registro) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (cantidad_de_bytes > 4 || cantidad_de_bytes < 1)
		return ERROR_EN_EJECUCION;

	int32_t valor_a_pushear;

	if (obtener_valor_del_registro(tcb, registro, &valor_a_pushear)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	char bytes[4];

	dividir_en_bytes(valor_a_pushear, bytes);

	return _push(tcb, cantidad_de_bytes, bytes);
}

/*
 * 	@DESC:	Obtiene tantos cantidad_de_bytes del stack y actualiza el cursor
 */
resultado_t _pop(tcb_t* tcb, int32_t cantidad_de_bytes, char bytes[4])
{
	if (leer_de_memoria(tcb->pid, tcb->cursor_stack, cantidad_de_bytes, bytes)
		== FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (mover_cursor_stack(tcb, -cantidad_de_bytes)
		== EXCEPCION_POR_POSICION_DE_STACK_INVALIDA)
		return ERROR_EN_EJECUCION;

	return OK;
}

/*
 * 	TAKE [Número], [Registro]
 *
 * 	Desapila los primeros bytes, indicado por el número, del stack hacia el registro.
 * 	Modifica el valor del registro cursor de stack de forma acorde.
 */
resultado_t take(tcb_t* tcb)
{
	int32_t cantidad_de_bytes;
	char registro;

	if (leer_numero(tcb, &cantidad_de_bytes) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (leer_registro(tcb, &registro) == FALLO_LECTURA_DE_MEMORIA)
		return ERROR_EN_EJECUCION;

	if (cantidad_de_bytes > 4 || cantidad_de_bytes < 1)
		return ERROR_EN_EJECUCION;

	char bytes[4];

	if (_pop(tcb, cantidad_de_bytes, bytes) == ERROR_EN_EJECUCION)
		return ERROR_EN_EJECUCION;

	int32_t valor;

	unir_bytes(&valor, bytes);

	if (actualizar_valor_del_registro(tcb, registro, valor)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return ERROR_EN_EJECUCION;

	return OK;
}

/*
 * 	XXXX
 *
 * 	Finaliza la ejecución.
 */
resultado_t xxxx(tcb_t* tcb)
{
	return FIN_EJECUCION;
}

/*
 * 	MALC
 *
 *	Reserva una cantidad de memoria especificada por el registro A.
 *	La direccion de esta se almacena en el registro A.
 *	Crea en la MSP un nuevo segmento del tamaño especificado
 *		asociado al programa en ejecución.
 */
resultado_t malc(tcb_t* tcb)
{
	if (!es_tcb_kernel(tcb))
	{
		return ERROR_EN_EJECUCION;
	}

	int32_t cantidad_de_bytes_a_pedir = obtener_valor_registro_a(tcb);
	direccion direccion;

	if (crear_segmento(tcb->pid, cantidad_de_bytes_a_pedir, &direccion)
		== FALLO_CREACION_DE_SEGMENTO)
	{
		return ERROR_EN_EJECUCION;
	}

	actualizar_registro_a(tcb, direccion);

	return OK;
}

/*
 * 	FREE
 *
 * 	Libera la memoria apuntada por el registro A.
 * 	Solo se podrá liberar memoria alocada por la instrucción de MALC.
 * 	Destruye en la MSP el segmento indicado en el registro A.
 */
resultado_t _free(tcb_t* tcb)
{
	if (!es_tcb_kernel(tcb))
	{
		return ERROR_EN_EJECUCION;
	}

	direccion direccion = obtener_valor_registro_a(tcb);

	if (destruir_segmento(tcb->pid, direccion) == FALLO_DESTRUCCION_DE_SEGMENTO)
	{
		return ERROR_EN_EJECUCION;
	}

	return OK;
}

/*
 * 	@DESC:	Le manda un mensaje al kernel para que pida por consola un numero devuelto en numero_ingresado
 */
resultado_t _pedir_por_consola_numero(tcb_t* tcb, int32_t* numero_ingresado)
{
	char* buffer = malloc(sizeof(char) * 4);

	uint32_t cantidad_de_bytes_leidos;

	if (comunicar_entrada_estandar(tcb, 4, &cantidad_de_bytes_leidos, buffer,
		ENTERO) != OK)
	{
		free(buffer);
		return ERROR_EN_EJECUCION;
	}

	unir_bytes(numero_ingresado, buffer);

	free(buffer);

	return OK;
}

/*
 * 	INNN
 *
 * 	Pide por consola del programa que se ingrese un número,
 * 		con signo entre –2.147.483.648 y 2.147.483.647. (32 bits) (4 bytes)
 * 	El mismo será almacenado en el registro A.
 * 	Invoca al servicio correspondiente en el proceso Kernel.
 */
resultado_t innn(tcb_t* tcb)
{
	if (!es_tcb_kernel(tcb))
	{
		return ERROR_EN_EJECUCION;
	}

	int32_t numero_ingresado;

	if (_pedir_por_consola_numero(tcb, &numero_ingresado) != OK)
	{
		return ERROR_EN_EJECUCION;
	}

	actualizar_registro_a(tcb, numero_ingresado);

	return OK;
}

/*
 * 	@DESC: 	Le manda un mensaje a kernel para que pida una cadena por consola.
 * 			El tamaño es maximo es cantidad_de_bytes_maximos.
 * 			La cadena la guarda en la MSP desde la direccion "direccion".
 * 	@RETURN:
 * 		OK: pudo completar la operacion
 * 		ERROR_EN_EJECUCION: hubo un fallo al escribir en memoria
 *
 */
resultado_t _pedir_por_consola_cadena(tcb_t* tcb, int32_t direccion,
	int32_t cantidad_de_bytes_maximos)
{
	char* buffer = malloc(cantidad_de_bytes_maximos);

	uint32_t cantidad_de_bytes_leidos;

	if (comunicar_entrada_estandar(tcb, cantidad_de_bytes_maximos,
		&cantidad_de_bytes_leidos, buffer, CADENA) != OK)
	{
		free(buffer);
		return ERROR_EN_EJECUCION;
	}

	if (escribir_en_memoria(tcb->pid, direccion, cantidad_de_bytes_leidos,
		buffer) == FALLO_ESCRITURA_EN_MEMORIA)
	{
		free(buffer);
		return ERROR_EN_EJECUCION;
	}

	free(buffer);

	return OK;
}

/*
 * 	INNC
 *
 * 	Pide por consola del programa que se ingrese una cadena no más larga de lo indicado
 * 		por el registro B.
 * 	La misma será almacenada en la posición de memoria apuntada por el registro A.
 * 	Invoca al servicio correspondiente en el proceso Kernel.
 */
resultado_t innc(tcb_t* tcb)
{
	if (!es_tcb_kernel(tcb))
	{
		return ERROR_EN_EJECUCION;
	}

	int32_t direccion_de_nueva_cadena = obtener_valor_registro_a(tcb);

	int32_t cantidad_de_bytes_maxima = obtener_valor_registro_b(tcb);

	return _pedir_por_consola_cadena(tcb, direccion_de_nueva_cadena,
		cantidad_de_bytes_maxima);
}

/*
 *	@DESC: Le manda al kernel el numero para que lo imprima por consola.
 */
resultado_t _imprimir_por_consola_numero(tcb_t* tcb, int32_t numero)
{
	char buffer[4];

	dividir_en_bytes(numero, buffer);

	if (comunicar_salida_estandar(tcb, 4, buffer) != OK)
	{
		return ERROR_EN_EJECUCION;
	}

	return OK;
}

/*
 * 	OUTN
 *
 * 	Imprime por consola del programa el número, con signo almacenado en el registro A.
 * 	Invoca al servicio correspondiente en el proceso Kernel.
 */
resultado_t outn(tcb_t* tcb)
{
	if (!es_tcb_kernel(tcb))
	{
		return ERROR_EN_EJECUCION;
	}

	int32_t numero_a_enviar = obtener_valor_registro_a(tcb);

	return _imprimir_por_consola_numero(tcb, numero_a_enviar);
}

/*
 *	@DESC: 	Le manda al kernel el una cadena para que lo imprima por consola.
 *			La cadena la obtiene de la msp desde la direccion_de_cadena.
 *	@RETURN:
 *		OK: pudo completar la operacion
 *		ERROR_EN_EJECUCION: fallo la lectura de memoria
 */
resultado_t _imprimir_por_consola_cadena(tcb_t* tcb,
	int32_t direccion_de_cadena, int32_t cantidad_de_bytes)
{
	char* buffer = malloc(cantidad_de_bytes);

	if (leer_de_memoria(tcb->pid, direccion_de_cadena, cantidad_de_bytes,
		buffer) == FALLO_LECTURA_DE_MEMORIA)
	{
		free(buffer);
		return ERROR_EN_EJECUCION;
	}

	if (comunicar_salida_estandar(tcb, cantidad_de_bytes, buffer) != OK)
	{
		free(buffer);
		return ERROR_EN_EJECUCION;
	}

	free(buffer);

	return OK;
}

/*
 * 	OUTC
 *
 * 	Imprime por consola del programa una cadena de tamaño indicado por el registro B
 * 		que se encuentra en la direccion apuntada por el registro A.
 * 	Invoca al servicio correspondiente en el proceso Kernel.
 */
resultado_t outc(tcb_t* tcb)
{
	if (!es_tcb_kernel(tcb))
	{
		return ERROR_EN_EJECUCION;
	}

	int32_t direccion_de_la_cadena = obtener_valor_registro_a(tcb);
	int32_t cantidad_de_bytes_de_la_cadena = obtener_valor_registro_b(tcb);

	return _imprimir_por_consola_cadena(tcb, direccion_de_la_cadena,
		cantidad_de_bytes_de_la_cadena);
}

// TODO eliminar (ya no hace falta)
///*
// * 	@DESC:	Crea un stack para el nuevo_tcb y se lo asigna
// */
//resultado_t _crear_stack(tcb_t* tcb)
//{
//	uint32_t tamano_stack;
//
//	pedir_al_kernel_tamanio_stack(&tamano_stack);
//
//	direccion nueva_base_stack;
//
//	if (crear_segmento(tcb->pid, tamano_stack, &nueva_base_stack)
//		== FALLO_CREACION_DE_SEGMENTO)
//		return ERROR_EN_EJECUCION;
//
//	actualizar_base_del_stack(tcb, nueva_base_stack);
//
//	return OK;
//}

// TODO eliminar (ya no hace falta)
///*
// * 	@DESC:	Copia todos los valores del stack del tcb al nuevo_tcb, actualizado los punteros.
// */
//resultado_t _clonar_stack(tcb_t* nuevo_tcb, tcb_t* tcb)
//{
//	uint32_t ocupacion_stack = obtener_ocupacion_stack(tcb);
//
//	char* buffer = malloc(ocupacion_stack);
//
//	if (leer_de_memoria(tcb->pid, tcb->base_stack, ocupacion_stack, buffer)
//		== FALLO_LECTURA_DE_MEMORIA)
//	{
//		free(buffer);
//		return ERROR_EN_EJECUCION;
//	}
//
//	if (escribir_en_memoria(nuevo_tcb->pid, nuevo_tcb->base_stack,
//		ocupacion_stack, buffer) == FALLO_ESCRITURA_EN_MEMORIA)
//	{
//		free(buffer);
//		return ERROR_EN_EJECUCION;
//	}
//
//	free(buffer);
//
//	if (mover_cursor_stack(tcb, ocupacion_stack)
//		== EXCEPCION_POR_POSICION_DE_STACK_INVALIDA)
//		return ERROR_EN_EJECUCION;
//
//	return OK;
//}

// TODO eliminar (ya no hace falta)
resultado_t _obtener_nuevo_tid(tcb_t* tcb, direccion* nuevo_tid)
{
	if (pedir_tid_a_kernel(*tcb, nuevo_tid) == FALLO_COMUNICACION)
	{
		return ERROR_EN_EJECUCION;
	}

	return OK;
}

// TODO arreglar con kernel que va a pasar con crea
/*
 * 	CREA
 *
 * 	Crea un hilo, hijo del TCB que ejecutó la llamada al sistema correspondiente.
 * 	El nuevo hilo tendrá su Program Counter apuntado al número almacenado en el registro B.
 * 	El identificador del nuevo hilo se almacena en el registro A.
 * 	Para lograrlo debe:
 * 		generar un nuevo TCB como copia del TCB actual,
 * 		asignarle un nuevo TID correlativo al actual,
 * 		cargar en el Puntero de Instrucción la rutina donde comenzará a ejecutar el
 * 			nuevo hilo (registro B),
 * 		pasarlo de modo Kernel a modo Usuario,
 * 		duplicar el segmento de stack desde la base del stack hasta el cursor del stack,
 * 		asignar la base y cursor de forma acorde
 * 			(tal que la diferencia entre cursor y base se mantenga igual),
 * 		luego invocar al servicio correspondiente en el proceso Kernel
 * 			con el TCB recién generado.
 * 	De no tener espacio para crear el segmento de stack, deberá abortar el programa.
 */
resultado_t crea(tcb_t* tcb)
{
	if (!es_tcb_kernel(tcb))
	{
		return ERROR_EN_EJECUCION;
	}

	direccion nuevo_tid;
	if (_obtener_nuevo_tid(tcb, &nuevo_tid) == ERROR_EN_EJECUCION)
		return ERROR_EN_EJECUCION;

	tcb_t* nuevo_tcb = crear_tcb();

	clonar_tcb(nuevo_tcb, tcb);

	direccion nuevo_pc = obtener_valor_registro_b(tcb);

	actualizar_pc(nuevo_tcb, nuevo_pc);
	actualizar_tid(nuevo_tcb, nuevo_tid);
	actualizar_km(nuevo_tcb, false);

	actualizar_registro_a(tcb, nuevo_tid);

	// TODO eliminar (ya no hace falta)
//	if (_crear_stack(nuevo_tcb) == ERROR_EN_EJECUCION)
//		return ERROR_EN_EJECUCION;

// TODO eliminar (ya no hace falta)
//	if (_clonar_stack(nuevo_tcb, tcb) == ERROR_EN_EJECUCION)
//		return ERROR_EN_EJECUCION;

	if (comunicar_nuevo_tcb(nuevo_tcb) != OK)
	{
		return ERROR_EN_EJECUCION;
	}

	return OK;
}

/*
 * 	JOIN
 *
 * 	Bloquea el programa que ejecutó la llamada al sistema hasta que
 * 		el hilo con el identificador almacenado en el registro A haya finalizado.
 * 	Invoca al servicio correspondiente en el proceso Kernel.
 */
resultado_t join(tcb_t* tcb)
{
	if (!es_tcb_kernel(tcb))
	{
		return ERROR_EN_EJECUCION;
	}

	int32_t identificador_almacenado_en_a = obtener_valor_registro_a(tcb);

	if (comunicar_join(tcb->tid, identificador_almacenado_en_a) != OK)
	{
		return ERROR_EN_EJECUCION;
	}

	return OK;
}

/*
 * 	BLOK
 *
 * 	Bloquea el programa que ejecutó la llamada al sistema hasta que
 * 		el recurso apuntado por B se libere.
 * 	La evaluación y decisión de si el recurso está libre o no es hecha por
 * 		la llamada al sistema WAIT pre-compilada.
 */
resultado_t blok(tcb_t* tcb)
{
	if (!es_tcb_kernel(tcb))
	{
		return ERROR_EN_EJECUCION;
	}

	int32_t id_recurso_almacenado_en_b = obtener_valor_registro_b(tcb);

	if (comunicar_bloquear(tcb, id_recurso_almacenado_en_b) != OK)
	{
		return ERROR_EN_EJECUCION;
	}

	return OK;
}

/*
 * 	WAKE
 *
 * 	Desbloquea al primer programa bloqueado por el recurso apuntado por B.
 * 	La evaluación y decisión de si el recurso está libre o no es hecha por
 * 		la llamada al sistema SIGNAL pre-compilada.
 */
resultado_t wake(tcb_t* tcb)
{
	if (!es_tcb_kernel(tcb))
	{
		return ERROR_EN_EJECUCION;
	}

	int32_t id_recurso_almacenado_en_b = obtener_valor_registro_b(tcb);

	if (comunicar_despertar(tcb, id_recurso_almacenado_en_b) != OK)
	{
		return ERROR_EN_EJECUCION;
	}

	return OK;
}

void inicializar_dic_de_instrucciones()
{
	loggear_trace("Intento crear el diccionario de instrucciones");
	dic_instrucciones = dictionary_create();
	loggear_info("Creacion de diccionario con exito");

	loggear_trace("Intento cargar el diccionario de instrucciones");
	cargar_diccionario_de_instrucciones(dic_instrucciones);
	loggear_info("Cargadas todas las instrucciones en el dic de instrucciones");
}

void liberar_dic_de_instrucciones()
{
	loggear_trace("Intento liberar el diccionario de instrucciones");
	dictionary_destroy(dic_instrucciones);
	loggear_info("Diccionario de instrucciones liberado correctamente");
}

void obtener_funcion_segun_instruccion(resultado_t (*funcion)(tcb_t*),
	instruccion_t instruccion)
{
	loggear_trace("Busco instruccion %s en dic de instrucciones", instruccion);
	funcion = dictionary_get(dic_instrucciones, instruccion);
	loggear_trace("Instruccion a ejecutar %s encontrada", instruccion);
}

void cargar_diccionario_de_instrucciones(t_dictionary* dic)
{
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
