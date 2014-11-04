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
	// Mensajes del Kernel a Todos: 51 -> 100
	MENSAJE_INCOMPLETO = 51,
	RECIBIDO_OK = 52,

	// Mensajes de la Consola al Kernel: 101 -> 150
	SOY_PROGRAMA = 101,	// Se envia al conectarse
	TERMINAR_CONEXION = 102,

	// Mensajes del Kernel a la Consola: 151 -> 200
	BIENVENIDO = 151,		// Se envia como respuesta a SOY_PROGRAMA (101) o a SOY_CPU (204)
	SIN_MEMORIA = 152,
	MORITE = 153,			// Que muera la consola
	PEDI_CADENA = 154,		// Pedir una Cadena al usuario
	PEDI_ENTERO = 155,		// Pedir un Entero al usuario
	MOSTRA_CADENA = 156,


	// Mensajes del CPU al Kernel: 201 -> 250
	NUEVO_CPU = 201,
	MANDA_TCB = 202,
	TOMA_RESULTADO = 203,
	SOY_CPU = 204,
	DAME_TID = 205,

	// Servicios expuestos a la CPU
	INTERRUPCION = 205,
	ENTRADA_ESTANDAR = 206,
	SALIDA_ESTANDAR = 207,
	CREAR_HILO = 208,
	JOIN = 209,
	BLOQUEAR = 210,
	DESPERTAR = 211,

	// Mensajes del Kernel al CPU: 251 -> 300
	TOMA_TCB = 251,
	RESPUESTA_ENTRADA = 252,
	TOMA_TID = 253,

	// Mensajes del CPU a Memoria: 301 -> 350
	CREAME_UN_SEGMENTO = 301,
	DESTRUI_SEGMENTO = 302,
	LEE_DE_MEMORIA = 303,
	ESCRIBI_EN_MEMORIA = 304,

	// Mensajes de Memoria al CPU: 351 -> 400
	TOMA_SEGMENTO = 351,
	RESPUESTA_DESTRUCCION = 352,
	TOMA_BYTES = 353,
	RESPUESTA_ESCRITURA = 354,

} flag_t;

typedef enum {
	ENTERO = 1,
	CADENA = 2
} idetificador_tipo_t;

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
	tcb_t* tcb;
	int32_t informacion;
} pedido_con_resultado_t;

typedef struct pedido_nuevo_tid {
	flag_t flag;
	tcb_t* tcb;
} pedido_nuevo_tid_t;

typedef struct respuesta_de_nuevo_tcb {
	flag_t flag;
	tcb_t* tcb;
	uint32_t quantum;
} respuesta_de_nuevo_tcb_t;

typedef struct respuesta_nuevo_tid {
	flag_t flag;
	direccion tid;
} respuesta_nuevo_tid_t;

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
	uint32_t tamano;
	char* bytes_leido;
} respuesta_de_leer_de_memoria_t;

// MENSAJES DE SERVICIOS EXPUESTOS A LA CPU

typedef struct pedido_interrupcion {
	flag_t flag;
	tcb_t* tcb;
	direccion direccion_de_memoria;
} pedido_interrupcion_t;

typedef struct pedido_entrada_estandar {
	flag_t flag;
	uint32_t pid;
	uint32_t tid;
	idetificador_tipo_t identificador_de_tipo;
} pedido_entrada_estandar_t;

typedef struct pedido_salida_estadar {
	flag_t flag;
	uint32_t pid;
	uint32_t tamanio;
	char* cadena_de_texto;
	idetificador_tipo_t identificador_de_tipo;
} pedido_salida_estandar_t;

typedef struct pedido_crear_hilo {
	flag_t flag;
	tcb_t* tcb;
} pedido_crear_hilo_t;

typedef struct pedido_join {
	flag_t flag;
	uint32_t tid_llamador;
	uint32_t tid_esperador;
} pedido_join_t;

typedef struct pedido_bloquear {
	flag_t flag;
	tcb_t* tcb;
	uint32_t identificador_de_recurso;
} pedido_bloquear_t;

typedef struct pedido_despertar {
	flag_t flag;
	uint32_t identificador_de_recurso;
} pedido_despertar_t;

// RESPUESTAS DE SERVICIOS EXPUESTOS A LA CPU

typedef struct respuesta_entrada_estandar {
	flag_t flag;
	uint32_t pid;
	uint32_t tid;
	uint32_t tamanio;
	char* cadena;
} respuesta_entrada_estandar_t;

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

respuesta_t* deserializar_respuesta_t(char*);
char* serializar_respuesta_t(respuesta_t*);
uint32_t tamanio_respuesta_t_serializado();

pedido_con_resultado_t* deserializar_pedido_con_resultado_t(char*);
char* serializar_pedido_con_resultado_t(pedido_con_resultado_t*);
uint32_t tamanio_pedido_con_resultado_t_serializado();

// TODO implementar estos dos

pedido_nuevo_tid_t* deserializar_pedido_nuevo_tid_t(char*);
char* serializar_pedido_nuevo_tid_t(pedido_nuevo_tid_t*);
uint32_t tamanio_pedido_nuevo_tid_t_serializado();

respuesta_nuevo_tid_t* deserializar_respuesta_nuevo_tid_t(char*);
char* serializar_respuesta_nuevo_tid_t(respuesta_nuevo_tid_t*);
uint32_t tamanio_respuesta_nuevo_tid_t_serializado();

pedido_de_crear_segmento_t* deserializar_pedido_de_crear_segmento_t(char*);
char* serializar_pedido_de_crear_segmento_t(pedido_de_crear_segmento_t*);
uint32_t tamanio_pedido_de_crear_segmento_t_serializado();

pedido_de_destruir_segmento_t* deserializar_pedido_de_destruir_segmento_t(char*);
char* serializar_pedido_de_destruir_segmento_t(pedido_de_destruir_segmento_t*);
uint32_t tamanio_pedido_de_destruir_segmento_t_serializado();

pedido_de_leer_de_memoria_t* deserializar_pedido_de_leer_de_memoria_t(char*);
char* serializar_pedido_de_leer_de_memoria_t(pedido_de_leer_de_memoria_t*);
uint32_t tamanio_pedido_de_leer_de_memoria_t_serializado();

pedido_de_escribir_en_memoria_t* deserializar_pedido_de_escribir_en_memoria_t(char*);
char* serializar_pedido_de_escribir_en_memoria_t(pedido_de_escribir_en_memoria_t*);
uint32_t tamanio_pedido_de_escribir_en_memoria_t_serializado(uint32_t);

respuesta_de_leer_de_memoria_t* deserializar_respuesta_de_leer_de_memoria_t(char*);
char* serializar_respuesta_de_leer_de_memoria_t(respuesta_de_leer_de_memoria_t*);
uint32_t tamanio_respuesta_de_leer_de_memoria_t_serializado(uint32_t);

respuesta_de_crear_segmento_t* deserializar_respuesta_de_crear_segmento_t(char*);
char* serializar_respuesta_de_crear_segmento_t(respuesta_de_crear_segmento_t*);
uint32_t tamanio_respuesta_de_crear_segmento_t_serializado();

pedido_interrupcion_t* deserializar_pedido_interrupcion_t(char*);
char* serializar_pedido_interrupcion_t(pedido_interrupcion_t*);
uint32_t tamanio_pedido_interrupcion_t_serializado();

pedido_entrada_estandar_t* deserializar_pedido_entrada_estandar_t(char*);
char* serializar_pedido_entrada_estandar_t(pedido_entrada_estandar_t*);
uint32_t tamanio_pedido_entrada_estandar_t_serializado();

pedido_salida_estandar_t* deserializar_pedido_salida_estandar_t(char*);
char* serializar_pedido_salida_estandar_t(pedido_salida_estandar_t*);
uint32_t tamanio_pedido_salida_estandar_t_serializado(uint32_t);

pedido_crear_hilo_t* deserializar_pedido_crear_hilo_t(char*);
char* serializar_pedido_crear_hilo_t(pedido_crear_hilo_t*);
uint32_t tamanio_pedido_crear_hilo_t_serializado();

pedido_join_t* deserializar_pedido_join_t(char*);
char* serializar_pedido_join_t(pedido_join_t*);
uint32_t tamanio_pedido_join_t_serializado();

pedido_bloquear_t* deserializar_pedido_bloquear_t(char*);
char* serializar_pedido_bloquear_t(pedido_bloquear_t*);
uint32_t tamanio_pedido_bloquear_t_serializado();

pedido_despertar_t* deserializar_pedido_despertar_t(char*);
char* serializar_pedido_despertar_t(pedido_despertar_t*);
uint32_t tamanio_pedido_despertar_t_serializado();

respuesta_entrada_estandar_t* deserializar_respuesta_entrada_estandar_t(char*);
char* serializar_respuesta_entrada_estandar_t(respuesta_entrada_estandar_t*);
uint32_t tamanio_respuesta_entrada_estandar_t_serializado(uint32_t);

#endif /* MENSAJES_H_ */
