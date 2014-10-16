#include "instrucciones.h"

// todo cambiar todos los fallos y errores y excepciones por ERROR_EN_EJECUCION

/*
 * 	LOAD [Registro], [Numero]
 *
 * 	Carga en el registro el número dado
 */
resultado_t load(tcb_t* tcb)
{
	char registro;
	int32_t numero;

	obtener_registro(tcb, &registro);
	obtener_numero(tcb, &numero);

	return actualizar_valor_en_registro(tcb, registro, numero);
}

/*
 * 	GETM [Registro], [Registro]
 *
 * 	Carga en el primer registro el valor de memoria apuntado por el segundo registro
 */
resultado_t getm(tcb_t* tcb)
{ // creo que no esta bien
	char registro1;
	char registro2;
	int32_t valor;

	obtener_registro(tcb, &registro1);
	obtener_registro(tcb, &registro2);

	if (obtener_valor_de_registro(tcb, registro2, &valor)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, registro1, valor);
}

void _copiar_valores(int32_t numero, direccion direccion1,
	direccion direccion2, tcb_t* tcb)
{
	char* buffer = malloc(numero);
	leer_de_memoria(tcb->pid, direccion1, numero, buffer);
	escribir_en_memoria(tcb->pid, direccion2, numero, buffer);
	free(buffer);
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
	char registro1;
	char registro2;
	int32_t valor1;
	int32_t valor2;

	obtener_numero(tcb, &cantidad_de_bytes_a_copiar);
	obtener_registro(tcb, &registro1);
	obtener_registro(tcb, &registro2);

	if (obtener_valor_de_registro(tcb, registro1, &valor1)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
	if (obtener_valor_de_registro(tcb, registro2, &valor2)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;

	direccion direccion1 = valor1;
	direccion direccion2 = valor2;

	_copiar_valores(cantidad_de_bytes_a_copiar, direccion1, direccion2, tcb);

	return OK;
}
/*
 * 	MOVR [Registro], [Registro]
 *
 *	Copia el valor del segundo registro hacia el primero
 */
resultado_t movr(tcb_t* tcb)
{
	char registro1;
	char registro2;
	int32_t valor;

	obtener_registro(tcb, &registro1);
	obtener_registro(tcb, &registro2);

	if (obtener_valor_de_registro(tcb, registro2, &valor)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, registro1, valor);
}

resultado_t _funcion_operacion(tcb_t* tcb, int32_t operacion(int32_t, int32_t))
{
	char registro1;
	char registro2;
	int32_t valor1;
	int32_t valor2;

	obtener_registro(tcb, &registro1);
	obtener_registro(tcb, &registro2);

	if (obtener_valor_de_registro(tcb, registro1, &valor1)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
	if (obtener_valor_de_registro(tcb, registro2, &valor2)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, 'a', operacion(valor1, valor2));
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

	return _funcion_operacion(tcb, sumar);
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

	return _funcion_operacion(tcb, restar);
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

	return _funcion_operacion(tcb, multiplicar);
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

	return _funcion_operacion(tcb, modulo);
}

/*
 * 	DIVR [Registro], [Registro]
 *
 * 	Divide el valor del primer registro con el del segundo registro.
 * 	El resultado de la operación se almacena en elregistro A.
 */
resultado_t divr(tcb_t* tcb)
{
	char registro1;
	char registro2;
	int32_t valor1;
	int32_t valor2;

	obtener_registro(tcb, &registro1);
	obtener_registro(tcb, &registro2);

	if (obtener_valor_de_registro(tcb, registro1, &valor1)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
	if (obtener_valor_de_registro(tcb, registro2, &valor2)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;

	if (valor2 == 0)
		return EXCEPCION_DIVISION_POR_CERO;

	return actualizar_valor_en_registro(tcb, 'a', valor1 / valor2);
}

resultado_t _funcion_incr_decr(tcb_t* tcb, int32_t operacion(int32_t))
{
	char registro;
	int32_t valor;

	obtener_registro(tcb, &registro);

	if (obtener_valor_de_registro(tcb, registro, &valor)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, 'a', operacion(valor));
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

resultado_t _funcion_comparacion(tcb_t* tcb,
	int32_t comparador(int32_t, int32_t))
{
	char registro1;
	char registro2;
	int32_t valor1;
	int32_t valor2;

	obtener_registro(tcb, &registro1);
	obtener_registro(tcb, &registro2);

	if (obtener_valor_de_registro(tcb, registro1, &valor1)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;
	if (obtener_valor_de_registro(tcb, registro2, &valor2)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;

	return actualizar_valor_en_registro(tcb, 'a', comparador(valor1, valor2));
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
	int32_t valor;
	direccion base_de_codigo;

	obtener_registro(tcb, &registro);
	obtener_base_de_codigo(tcb, &base_de_codigo);

	if (obtener_valor_de_registro(tcb, registro, &valor)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;

	return actualizar_pc(tcb, base_de_codigo + valor);
}

resultado_t _funcion_de_salto(tcb_t* tcb, int32_t condicion(int32_t))
{
	int32_t offset;
	int32_t valor;
	direccion base_de_codigo;

	obtener_numero(tcb, &offset);
	obtener_valor_de_registro(tcb, 'a', &valor);
	obtener_base_de_codigo(tcb, &base_de_codigo);

	if (condicion(valor))
		return OK;

	return actualizar_pc(tcb, base_de_codigo + offset);
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

	/*
	 * Mandar un mensaje al kernel pidiendole que haga tal interrupcion
	 * Esperar una respuesta con el tcb actualizados
	 *
	 */
}

void _desplazar_bits(int32_t bits_a_desplazar, int32_t* valor)
{
	if (bits_a_desplazar > 0) {
		*valor = *valor >> bits_a_desplazar;
	} else {
		*valor = *valor << bits_a_desplazar;
	}
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
	char registro;
	int32_t bits_a_desplazar;
	int32_t valor;

	obtener_numero(tcb, &bits_a_desplazar);
	obtener_registro(tcb, &registro);

	if (obtener_valor_de_registro(tcb, registro, &valor)
		== EXCEPCION_NO_ENCONTRO_EL_REGISTRO)
		return EXCEPCION_NO_ENCONTRO_EL_REGISTRO;

	_desplazar_bits(bits_a_desplazar, &valor);

	return actualizar_valor_en_registro(tcb, registro, valor);
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

/*
 *  PUSH [Número], [Registro]
 *
 * 	Apila los primeros bytes, indicado por el número, del valor del registro hacia el stack.
 * 	Modifica el valor del registro cursor de stack de forma acorde.
 */
resultado_t push(tcb_t* tcb)
{ // TODO dar un vistazo porque no se si esta bien

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

	// todo validar que numero no sea mayor que 4
	char registro;
	int32_t cantidad_de_bytes;
	int32_t valor;

	obtener_numero(tcb, &cantidad_de_bytes);
	obtener_registro(tcb, &registro);
	obtener_valor_de_registro(tcb, registro, &valor);

	unsigned char bytes[4];

	bytes[0] = (valor >> 24) & 0xFF;
	bytes[1] = (valor >> 16) & 0xFF;
	bytes[2] = (valor >> 8) & 0xFF;
	bytes[3] = valor & 0xFF;

	escribir_en_memoria(tcb->pid, tcb->cursor_stack, cantidad_de_bytes, bytes); // todo agregar validacion

	tcb->cursor_stack = tcb->cursor_stack + cantidad_de_bytes; // todo pensar si hace falta encapsular

	return OK;
}

/*
 * 	TAKE [Número], [Registro]
 *
 * 	Desapila los primeros bytes, indicado por el número, del stack hacia el registro.
 * 	Modifica el valor del registro cursor de stack de forma acorde.
 */
resultado_t take(tcb_t* tcb)
{ // TODO dar un vistazo porque no se si esta bien
// todo validar que numero no sea mayor que 4
	char registro;
	int32_t cantidad_de_bytes;
	int32_t valor;

	obtener_numero(tcb, &cantidad_de_bytes);
	obtener_registro(tcb, &registro);

	unsigned char bytes[4];

	leer_de_memoria(tcb->pid, tcb->cursor_stack, cantidad_de_bytes, bytes); // todo agregar validacion

	tcb->cursor_stack = tcb->cursor_stack - cantidad_de_bytes; // todo pensar si hace falta encapsular

	unsigned char buffer[4];

	buffer[0] = bytes[3];
	buffer[1] = bytes[2];
	buffer[2] = bytes[1];
	buffer[3] = bytes[0];

	valor = *(int32_t *) buffer;

	/*
	 valor = buffer[0] |
	 ((int32_t)buffer[1] << 8 ) |
	 ((int32_t)buffer[2] << 16) |
	 ((int32_t)buffer[3] << 24)
	 */

	actualizar_valor_en_registro(tcb, registro, valor);

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

// todo verificar que las instrucciones de sistema solo las pueda ejecutar el tcb k

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
	int32_t bytes;

	obtener_valor_de_registro(tcb, 'a', &bytes);

	direccion direccion;
	if (crear_segmento(tcb->pid, bytes, &direccion)
		== FALLO_CREACION_DE_SEGMENTO)
		return FALLO_CREACION_DE_SEGMENTO;

	return actualizar_valor_en_registro(tcb, 'a', direccion);
}

/*
 * 	FREE
 *
 * 	Libera la memoria apuntada por el registro A.
 * 	Solo se podrá liberar memoria alocada por la instrucción de MALC.
 * 	Destruye en la MSP el segmento indicado en el registro A.
 */
resultado_t _free(tcb_t* tcb)
{ // todo falta verificar que la memoria alocada sea por instruccion MALC
	int32_t valor;

	obtener_valor_de_registro(tcb, 'a', &valor);

	direccion direccion = valor;

	return destruir_segmento(tcb->pid, direccion);
}

/*
 * 	INNN
 *
 * 	Pide por consola del programa que se ingrese un número,
 * 		con signo entre –2.147.483.648 y 2.147.483.647.
 * 	El mismo será almacenado en el registro A.
 * 	Invoca al servicio correspondiente en el proceso Kernel.
 */
resultado_t innn(tcb_t* tcb)
{
	return EXCEPCION_POR_INGRESO_TEXTO;

	/*
	 * Mandar mensaje a kernel diciendole que pida una cadena de texto
	 * Esperar que kernel te devuelva la cadena
	 *
	 */
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
	return EXCEPCION_POR_INGRESO_TEXTO_CON_TAMANO;

	/*
	 * Mandar mensaje a kernel diciendole que pida una cadena de texto con determinado valor
	 * Esperar que kernel te devuelva la cadena
	 *
	 */
}

/*
 * 	OUTN
 *
 * 	Imprime por consola del programa el número, con signo almacenado en el registro A.
 * 	Invoca al servicio correspondiente en el proceso Kernel.
 */
resultado_t outn(tcb_t* tcb)
{
	return EXCEPCION_POR_IMPRIMIR_TEXTO;

	/*
	 * Mandar mensaje a kernel diciendole que muestre un mensaje por pantalla
	 * Esperar a que kernel te diga que salio bien
	 *
	 */
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
	return EXCEPCION_POR_IMPRIMIR_TEXTO_CON_TAMANO;

	/* Mandar mensaje a kernel diciendole que muestre un mensaje por pantalla
	 * Esperar a que kernel te diga que salio bien
	 */
}

void _pedir_a_kernel_tamano_stack(uint32_t* tamano_stack) {

}

void _clonar_tcb(tcb_t* nuevo_tcb, tcb_t* tcb)
{
	memcpy(&*nuevo_tcb, tcb, sizeof(tcb_t));
}

void _crear_stack(tcb_t* nuevo_tcb)
{
	uint32_t tamano_stack;
	direccion nueva_base_stack;
	_pedir_a_kernel_tamano_stack(&tamano_stack);
	crear_segmento(nuevo_tcb->pid, tamano_stack, &nueva_base_stack);
	nuevo_tcb->base_stack = nueva_base_stack;
}

void _clonar_stack(tcb_t* nuevo_tcb, tcb_t* tcb)
{
	uint32_t ocupacion_stack = tcb->cursor_stack - tcb->base_stack;
	char* buffer = malloc(ocupacion_stack);
	leer_de_memoria(tcb->pid, tcb->base_stack, ocupacion_stack, buffer);
	escribir_en_memoria(nuevo_tcb->pid, nuevo_tcb->base_stack, ocupacion_stack,
		buffer);
	nuevo_tcb->cursor_stack = nuevo_tcb->base_stack + ocupacion_stack;
}

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
	tcb_t nuevo_tcb;

	int32_t nuevo_pc;
	obtener_valor_de_registro(tcb, 'b', &nuevo_pc);

	// COPIO EL TCB AL NUEVO TCB TAL CUAL
	_clonar_tcb(&nuevo_tcb, tcb);

	// LE CAMBIO ALGUNOS VALORES SEGÚN LOS REQUERIMIENTOS
	nuevo_tcb.pc = nuevo_pc;
	nuevo_tcb.tid = tcb->tid + 1; // TODO: corroborar que no exista otro tcb con ese tid
	nuevo_tcb.km = false;
	actualizar_valor_en_registro(tcb, 'a', nuevo_tcb.tid);

	// CREA UN NUEVO STACK PARA EL HILO 2
	_crear_stack(&nuevo_tcb);

	// COPIO EL STACK DEL HILO 1 AL HILO 2
	_clonar_stack(&nuevo_tcb, tcb);

	return OK;

	/*
	 * 	Mandar tcb a kernel
	 * 	Esperar que te devuelva un ok
	 */
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
	return EXCEPCION_POR_JOIN;

	/* Mandar mensaje a kernel
	 * Esperar que te devuelva un ok
	 */
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
	return EXCEPCION_POR_BLOQUEO;

	/* Mandar mensaje a kernel
	 * Esperar que te devuelva un ok
	 */
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
	return EXCEPCION_POR_DESBLOQUEO;

	/* Mandar mensaje a kernel
	 * Esperar que te devuelva un ok
	 */
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
