/*
 * resultados.h
 *
 *  Created on: 27/09/2014
 *      Author: utnso
 */

#ifndef RESULTADOS_H_
#define RESULTADOS_H_

typedef enum {
	OK,
	FALLO,
	PRUEBA,
	TERMINO,

	// FALLOS EN CPU: 101 -> 151
	NO_ENCONTRO_EL_REGISTRO = 101,
	FALLO_CONEXION = 102,
	FALLO_COMUNICACION = 103,
	FALLO_PEDIDO_DE_TCB = 104,
	FALLO_CREACION_DE_SEGMENTO = 105,
} resultado_t;

#endif /* RESULTADOS_H_ */
