/*
 * mensajes.h
 *
 *  Created on: 24/09/2014
 *      Author: utnso
 */

#include "tcb.h"

#ifndef MENSAJES_H_
#define MENSAJES_H_

typedef enum {
	// Mensajes de la Consola al Kernel: 101 -> 150
	NUEVO_PROGRAMA = 101,
	TERMINAR_CONEXION = 102,

	// Mensajes del Kernel a la Consola: 151 -> 200


	// Mensajes del CPU al Kernel: 201 -> 250
	NUEVO_CPU = 201,
	TOMA_RESULTADO = 202,

	// Mensajes del Kernel al CPU: 251 -> 300
	RESPONDO_TCB = 251,

	// ORDENAME SANTI
	/*
	 * challenge = new Challenge();
	 * santi = new Persona();
	 * santi.accept(challenge);
	 * martin = new Persona();
	 * santi.sort(martin);
	 */
	EXITO,
	MANDA_TCB,
	TOMA_TCB,
	CREAME_UN_SEGMENTO,
	TOMA_SEGMENTO,
	ERROR_MEMORIA_LLENA,
	DESTRUI_SEGMENTO,
	LEE_DE_MEMORIA,
	ERROR_VIOLACION_DE_SEGMENTO,
	ESCRIBI_EN_MEMORIA
} flag_t;

typedef struct pedido {
	flag_t flag;
} pedido_t;

typedef struct respuesta {
	flag_t flag;
} respuesta_t;

typedef struct pedido_con_resultado {
	flag_t flag;
	uint32_t resultado;
	tcb_t tcb;
} pedido_con_resultado_t;

typedef struct respuesta_de_nuevo_tcb {
	flag_t flag;
	tcb_t tcb;
	uint32_t quantum;
} respuesta_de_nuevo_tcb_t;

// OPCION 1

typedef struct pedido_de_crear_segmento {
	flag_t flag;
	uint32_t pid;
	uint32_t tamano;
} pedido_de_crear_segmento_t;

typedef struct pedido_de_destruir_segmento {
	flag_t flag;
	uint32_t pid;
	direccion direccion_virtual;
} pedido_de_destruir_segmento_t;

typedef struct pedido_de_leer_de_memoria {
	flag_t flag;
	uint32_t pid;
	direccion direccion_virtual;
	uint32_t tamano;
} pedido_de_leer_de_memoria_t;

typedef struct pedido_de_escribir_en_memoria {
	flag_t flag;
	uint32_t pid;
	direccion direccion_virtual;
	char* bytes_a_escribir;
	uint32_t tamano;
} pedido_de_escribir_en_memoria_t;

typedef struct respuesta_de_crear_segmento {
	flag_t flag;
	direccion direccion_virtual;
} respuesta_de_crear_segmento_t;

typedef struct respuesta_de_leer_de_memoria {
	flag_t flag;
	char* bytes_leido;
} respuesta_de_leer_de_memoria_t;

// OPCION 2

typedef struct pedido_de_cpu_a_memoria {
	flag_t flag;
	uint32_t pid;
	direccion direccion_virtual;
	char* bytes_a_escribir;
	uint32_t tamano;
} pedido_de_cpu_a_memoria_t;

typedef struct respuesta_de_memoria_a_cpu {
	flag_t flag;
	direccion direccion_virtual;
	char* bytes_leido;
	uint32_t tamano;
} respuesta_de_memoria_a_cpu_t;

// OPCION 3

typedef struct mensaje_cpu_y_memoria {
	flag_t flag;
	uint32_t pid;
	direccion direccion_virtual;
	char* bytes_leido;
	uint32_t tamano;
} mensaje_cpu_y_memoria_t;


/**
 * Mensaje es el chorro de bytes recibidos a traves del socket.
 *
 * @RETURNS: El codigo de operacion solicitado
 */
flag_t codigo_operacion(char* mensaje);

/**
 * @RETURNS: El tamanio de un flag_t (igual a sizeof(flag_t))
 */
uint32_t tamanio_flagt();

#endif /* MENSAJES_H_ */
