/*
 * mensajes.h
 *
 *  Created on: 24/09/2014
 *      Author: utnso
 */

#include <string.h>
#include "tcb.h"

#ifndef MENSAJES_H_
#define MENSAJES_H_

typedef enum {
	// Mensajes de la Consola al Kernel: 101 -> 150
	SOY_PROGRAMA = 101,	// Se envia al conectarse
	TERMINAR_CONEXION = 102,

	// Mensajes del Kernel a la Consola: 151 -> 200
	BIENVENIDO = 151,		// Se envia como respuesta a SOY_PROGRAMA (101) o a SOY_CPU (204)


	// Mensajes del CPU al Kernel: 201 -> 250
	NUEVO_CPU = 201,
	MANDA_TCB = 202,
	TOMA_RESULTADO = 203,
	SOY_CPU = 204,		// Se envia al conectarse

	// Mensajes del Kernel al CPU: 251 -> 300
	TOMA_TCB = 251,

	// Mensajes del CPU a Memoria: 301 -> 350
	CREAME_UN_SEGMENTO = 301,
	DESTRUI_SEGMENTO = 302,
	LEE_DE_MEMORIA = 303,
	ESCRIBI_EN_MEMORIA = 304,

	// Mensajes de Memoria al CPU: 351 -> 400
	TOMA_SEGMENTO = 351,
	RESPUESTA_DESTRUCCION = 352,
	TOMA_BYTES = 353,
	RESPUETA_ESCRITURA = 354,

} flag_t;

typedef struct pedido {
	flag_t flag;
} pedido_t;

typedef struct respuesta {
	flag_t flag;
	resultado_t resultado;
} respuesta_t;

typedef struct pedido_con_resultado {
	flag_t flag;
	resultado_t resultado;
	tcb_t tcb;
	int32_t informacion;
} pedido_con_resultado_t;

typedef struct respuesta_de_nuevo_tcb {
	flag_t flag;
	tcb_t* tcb;
	uint32_t quantum;
} respuesta_de_nuevo_tcb_t;

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
	resultado_t resultado;
	direccion direccion_virtual;
} respuesta_de_crear_segmento_t;

typedef struct respuesta_de_leer_de_memoria {
	flag_t flag;
	resultado_t resultado;
	char* bytes_leido;
} respuesta_de_leer_de_memoria_t;

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


respuesta_de_nuevo_tcb_t* deserializar_respuesta_de_nuevo_tcb_t(char*);
char* serializar_respuesta_de_nuevo_tcb_t(respuesta_de_nuevo_tcb_t*);
uint32_t tamanio_respuesta_de_nuevo_tcb_t_serializado();

pedido_t* deserializar_pedido_t(char*);
char* serializar_pedido_t(pedido_t*);
uint32_t tamanio_pedido_t_serializado();

#endif /* MENSAJES_H_ */
