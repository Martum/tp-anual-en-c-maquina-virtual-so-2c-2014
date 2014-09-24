/*
 * mensajes.h
 *
 *  Created on: 24/09/2014
 *      Author: utnso
 */

#include <stdbool.h>
#include <stdint.h>

#ifndef MENSAJES_H_
#define MENSAJES_H_

typedef uint32_t direccion;

typedef struct tcb {
	uint32_t pid;
	uint32_t tid;
	bool km;
	direccion base_codigo;
	direccion tamanio_codigo;
	direccion pc;
	direccion base_stack;
	direccion cursor_stack;
	int32_t a, b, c, d, e, f;
} tcb_t;

typedef enum {
	// Mensajes de la Consola al Kernel: 101 -> 150
	NUEVO_PROGRAMA = 101,
	TERMINAR_CONEXION = 102,

	// Mensajes del Kernel a la Consola: 151 -> 200


	// Mensajes del CPU al Kernel: 201 -> 250
	NUEVO_CPU = 201,

	// ORDENAME SANTI
	EXITO,
	MANDA_TCB,
	TOMA_TCB,
	CREAME_UN_SEGMENTO,
	TOMA_SEGMENTO,
	ERROR_MEMORIA_LLENA,
	DESTRUI_SEGMENTO,
	LEE_DE_MEMORIA,
	ERROR_VIOLACION_DE_SEGMENTO,
	ESCRIBI_EN_MEMORIA,
	TOMA_RESULTADO,
} flag_t;

typedef struct mensaje_request {
	flag_t flag;
} mensaje_request_t;

typedef struct mensaje_response {
	flag_t flag;
} mensaje_response_t;

typedef struct mensaje_request_resultado {
	flag_t flag;
	uint32_t resultado;
} mensaje_request_resultado_t;

typedef struct mensaje_request_crear_segmento {
	flag_t flag;
	uint32_t pid;
	uint32_t tamano;
} mensaje_request_crear_segmento_t;

typedef struct mensaje_request_destruir_segmento {
	flag_t flag;
	uint32_t pid;
	direccion direccion_virtual;
} mensaje_request_destruir_segmento_t;

typedef struct mensaje_request_lee_de_memoria {
	flag_t flag;
	uint32_t pid;
	direccion direccion_virtual;
	uint32_t tamano;
} mensaje_request_lee_de_memoria_t;

typedef struct mensaje_request_escribi_en_memoria {
	flag_t flag;
	uint32_t pid;
	direccion direccion_virtual;
	char* bytes_a_escribir;
	uint32_t tamano;
} mensaje_request_escribi_en_memoria_t;

typedef struct mensaje_response_tcb {
	flag_t flag;
	tcb_t tcb;
	uint32_t quantum;
} mensaje_response_tcb_t;

typedef struct mensaje_response_crear_segmento {
	flag_t flag;
	direccion direccion_virtual;
} mensaje_response_crear_segmento_t;

typedef struct mensaje_response_lee_de_memoria {
	flag_t flag;
	char* bytes_leido;
} mensaje_response_lee_de_memoria_t;

#endif /* MENSAJES_H_ */
