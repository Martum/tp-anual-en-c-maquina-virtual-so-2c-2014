/*
 * mensajes.c
 *
 *  Created on: 24/9/2014
 *      Author: martin
 */

#include "mensajes.h"
#include <stdlib.h>

flag_t codigo_operacion(char* mensaje)
{
	flag_t cod_op;
	memcpy(&cod_op, mensaje, sizeof(flag_t));

	return cod_op;
}

uint32_t tamanio_flagt()
{
	return sizeof(flag_t);
}

// COMIENZO DE PEDIDO

char* serializar_pedido_t(pedido_t* pedido)
{
	char* bytes = malloc(tamanio_pedido_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	return bytes;
}

pedido_t* deserializar_pedido_t(char* chorro)
{
	pedido_t* pedido = malloc(sizeof(pedido_t));

	uint32_t offset = 0;
	memcpy(&pedido->flag, chorro + offset, sizeof(flag_t));

	return pedido;
}

uint32_t tamanio_pedido_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);

	return t;
}

// FIN DE PEDIDO

// COMIENZO DE RESPUESTA

char* serializar_respuesta_t(respuesta_t* rta)
{
	char* bytes = malloc(tamanio_pedido_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &rta->flag, sizeof(flag_t));

	return bytes;
}

respuesta_t* deserializar_respuesta_t(char* chorro)
{
	respuesta_t* rta = malloc(sizeof(pedido_t));

	uint32_t offset = 0;
	memcpy(&rta->flag, chorro + offset, sizeof(flag_t));

	return rta;
}

uint32_t tamanio_respuesta_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	return t;
}

// FIN DE RESPUESTA

// COMIENZO RESPUESTA DE NUEVO TCB

char* serializar_respuesta_de_nuevo_tcb_t(
	respuesta_de_nuevo_tcb_t* rta_nuevo_tcb)
{
	char* bytes = malloc(tamanio_respuesta_de_nuevo_tcb_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &rta_nuevo_tcb->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &rta_nuevo_tcb->quantum, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, serializar_tcb(rta_nuevo_tcb->tcb),
		tamanio_tcb_serializado());

	return bytes;
}

respuesta_de_nuevo_tcb_t* deserializar_respuesta_de_nuevo_tcb_t(char* chorro)
{
	respuesta_de_nuevo_tcb_t* rta_nuevo_tcb = malloc(
		sizeof(respuesta_de_nuevo_tcb_t));

	uint32_t offset = 0;
	memcpy(&rta_nuevo_tcb->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&rta_nuevo_tcb->quantum, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	rta_nuevo_tcb->tcb = malloc(sizeof(tcb_t));
	memcpy(rta_nuevo_tcb->tcb, deserializar_tcb(chorro + offset),
		sizeof(tcb_t));

	return rta_nuevo_tcb;
}

uint32_t tamanio_respuesta_de_nuevo_tcb_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(uint32_t);
	t += tamanio_tcb_serializado();

	return t;
}

// FIN DE RESPUESTA DE NUEVO TCB

// COMPIEZO DE PEDIDO CON RESULTADO

char* serializar_pedido_con_resultado_t(
	pedido_con_resultado_t* pedido_con_resultado)
{
	char* bytes = malloc(tamanio_pedido_con_resultado_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido_con_resultado->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido_con_resultado->informacion, sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(bytes + offset, &pedido_con_resultado->resultado,
		sizeof(resultado_t));

	offset += sizeof(resultado_t);
	memcpy(bytes + offset, serializar_tcb(pedido_con_resultado->tcb),
		tamanio_tcb_serializado());

	return bytes;
}

pedido_con_resultado_t* deserializar_pedido_con_resultado_t(char* chorro)
{
	pedido_con_resultado_t* pedido_con_resultado = malloc(
		sizeof(pedido_con_resultado_t));

	uint32_t offset = 0;
	memcpy(&pedido_con_resultado->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&pedido_con_resultado->informacion, chorro + offset,
		sizeof(int32_t));

	offset += sizeof(int32_t);
	memcpy(&pedido_con_resultado->resultado, chorro + offset,
		sizeof(resultado_t));

	offset += sizeof(resultado_t);
	pedido_con_resultado->tcb = malloc(sizeof(tcb_t));
	memcpy(pedido_con_resultado->tcb, deserializar_tcb(chorro + offset),
		sizeof(tcb_t));

	return pedido_con_resultado;
}

uint32_t tamanio_pedido_con_resultado_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(int32_t);
	t += sizeof(resultado_t);
	t += tamanio_tcb_serializado();

	return t;
}

// FIN DE PEDIDO CON RESULTADO

// COMIEZO DE PEDIDO DE CREAR SEGMENTO

char* serializar_pedido_de_crear_segmento_t(pedido_de_crear_segmento_t* pedido)
{
	char* bytes = malloc(tamanio_pedido_de_crear_segmento_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido->pid, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &pedido->tamano, sizeof(uint32_t));

	return bytes;
}

pedido_de_crear_segmento_t* deserializar_pedido_de_crear_segmento_t(
	char* chorro)
{
	pedido_de_crear_segmento_t* pedido_crear_segmento = malloc(
		sizeof(pedido_de_crear_segmento_t));

	uint32_t offset = 0;
	memcpy(&pedido_crear_segmento->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&pedido_crear_segmento->pid, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&pedido_crear_segmento->tamano, chorro + offset, sizeof(uint32_t));

	return pedido_crear_segmento;
}

uint32_t tamanio_pedido_de_crear_segmento_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(uint32_t);
	t += sizeof(uint32_t);

	return t;
}

// FIN DE PEDIDO DE CREAR SEGMENTO


// COMIENZO DE PEDIDO DE DESTRUIR SEGMENTO

char* serializar_pedido_de_destruir_segmento_t(
	pedido_de_destruir_segmento_t* pedido)
{
	char* bytes = malloc(tamanio_pedido_de_crear_segmento_t_serializado());

	uint32_t offset = 0;	// BUG. FLAG_T tiene que estar primero
	memcpy(bytes + offset, &pedido->direccion_virtual, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido->pid, sizeof(uint32_t));

	return bytes;
}

pedido_de_destruir_segmento_t* deserializar_pedido_de_destruir_segmento_t(
	char* chorro)
{
	pedido_de_destruir_segmento_t* pedido_destruir_segmento = malloc(
		sizeof(pedido_de_destruir_segmento_t));

	uint32_t offset = 0;	// BUG. FLAG_T tiene que estar primero
	memcpy(&pedido_destruir_segmento->direccion_virtual, chorro + offset,
		sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(&pedido_destruir_segmento->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&pedido_destruir_segmento->pid, chorro + offset, sizeof(uint32_t));

	return pedido_destruir_segmento;
}

uint32_t tamanio_pedido_de_destruir_segmento_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(direccion);
	t += sizeof(flag_t);
	t += sizeof(uint32_t);

	return t;
}

// FIN DE PEDIDO DE DESTRUIR SEGMENTO


// COMIENZO DE PEDIDO DE LEER DE MEMORIA

char* serializar_pedido_de_leer_de_memoria_t(pedido_de_leer_de_memoria_t* pedido){

	char* bytes = malloc(tamanio_pedido_de_leer_de_memoria_t_serializado());

	uint32_t offset = 0;	// BUG. FLAG_T tiene que estar primero
	memcpy(bytes + offset, &pedido->direccion_virtual, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido->pid, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &pedido->tamano, sizeof(uint32_t));

	return bytes;
}


pedido_de_leer_de_memoria_t* deserializar_pedido_de_leer_de_memoria_t(char* chorro){
	pedido_de_leer_de_memoria_t* pedido_de_leer_de_memoria = malloc(
			sizeof(pedido_de_leer_de_memoria_t));

	uint32_t offset = 0;	// BUG. FLAG_T tiene que estar primero
	memcpy(&pedido_de_leer_de_memoria->direccion_virtual, chorro + offset,
		sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(&pedido_de_leer_de_memoria->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&pedido_de_leer_de_memoria->pid, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&pedido_de_leer_de_memoria->tamano, chorro + offset, sizeof(uint32_t));

	return pedido_de_leer_de_memoria;
}

uint32_t tamanio_pedido_de_leer_de_memoria_t_serializado(){
	uint32_t t = 0;
	t += sizeof(direccion);
	t += sizeof(flag_t);
	t += sizeof(uint32_t);
	t += sizeof(uint32_t);

	return t;
}

// FIN DE PEDIDO DE LEER DE MEMORIA


// COMIENZO DE PEDIDO DE ESCRIBIR EN MEMORIA

char* serializar_pedido_de_escribir_en_memoria_t(
	pedido_de_escribir_en_memoria_t* pedido)
{
	char* bytes = malloc(tamanio_pedido_de_escribir_en_memoria_t_serializado(pedido->tamano));

	uint32_t offset = 0;	// BUG. FLAG_T tiene que estar primero
	memcpy(bytes + offset, &pedido->direccion_virtual, sizeof(direccion));

	offset += sizeof(direccion);
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido->pid, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &pedido->tamano, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &pedido->bytes_a_escribir, pedido->tamano);

	return bytes;
}

pedido_de_escribir_en_memoria_t* deserializar_pedido_de_escribir_en_memoria_t(
	char* chorro)
{
	pedido_de_escribir_en_memoria_t* pedido = malloc(
				sizeof(pedido_de_escribir_en_memoria_t));

		uint32_t offset = 0;	// BUG. FLAG_T tiene que estar primero
		memcpy(&pedido->direccion_virtual, chorro + offset,
			sizeof(direccion));

		offset += sizeof(direccion);
		memcpy(&pedido->flag, chorro + offset, sizeof(flag_t));

		offset += sizeof(flag_t);
		memcpy(&pedido->pid, chorro + offset, sizeof(uint32_t));

		offset += sizeof(uint32_t);
		memcpy(&pedido->tamano, chorro + offset, sizeof(uint32_t));

		offset += sizeof(uint32_t);
		memcpy(&pedido->bytes_a_escribir, chorro + offset, pedido->tamano);

		return pedido;
}

uint32_t tamanio_pedido_de_escribir_en_memoria_t_serializado(uint32_t tamano)
{
	uint32_t t = 0;
	t += sizeof(direccion);
	t += sizeof(flag_t);
	t += sizeof(uint32_t);
	t += sizeof(uint32_t);
	t += tamano;

	return t;
}

// FIN DE PEDIDO DE ESCRIBIR EN MEMORIA


// COMIENZO DE RESPUESTA DE LEER DE MEMORIA

char* serializar_respuesta_de_leer_de_memoria_t(
	respuesta_de_leer_de_memoria_t* pedido)
{
	char* bytes = malloc(tamanio_respuesta_de_leer_de_memoria_t_serializado(pedido->tamano));

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido->resultado, sizeof(resultado_t));

	offset += sizeof(resultado_t);
	memcpy(bytes + offset, &pedido->tamano, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &pedido->bytes_leido, pedido->tamano);

	return bytes;
}

respuesta_de_leer_de_memoria_t* deserializar_respuesta_de_leer_de_memoria_t(
	char* chorro)
{
	respuesta_de_leer_de_memoria_t* respuesta = malloc(sizeof(respuesta_de_leer_de_memoria_t));

	uint32_t offset = 0;
	memcpy(&respuesta->flag, chorro + offset,
		sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&respuesta->resultado, chorro + offset, sizeof(resultado_t));

	offset += sizeof(resultado_t);
	memcpy(&respuesta->tamano, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&respuesta->bytes_leido, chorro + offset, respuesta->tamano);

	return respuesta;
}

uint32_t tamanio_respuesta_de_leer_de_memoria_t_serializado(uint32_t tamano)
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(resultado_t);
	t += sizeof(uint32_t);
	t += tamano;

	return t;
}

// FIN DE RESPUESTA DE LEER DE MEMORIA


// COMIENZO DE RESPUESTA DE CREAR SEGMENTO

char* serializar_respuesta_de_crear_segmento_t(
	respuesta_de_crear_segmento_t* pedido)
{
	char* bytes = malloc(tamanio_respuesta_de_crear_segmento_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido->resultado, sizeof(resultado_t));

	offset += sizeof(resultado_t);
	memcpy(bytes + offset, &pedido->direccion_virtual, sizeof(direccion));

	return bytes;
}

respuesta_de_crear_segmento_t* deserializar_respuesta_de_crear_segmento_t(
	char* chorro)
{
	respuesta_de_crear_segmento_t* respuesta = malloc(sizeof(respuesta_de_crear_segmento_t));

	uint32_t offset = 0;
	memcpy(&respuesta->flag, chorro + offset,
		sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&respuesta->resultado, chorro + offset, sizeof(resultado_t));

	offset += sizeof(resultado_t);
	memcpy(&respuesta->direccion_virtual, chorro + offset, sizeof(uint32_t));

	return respuesta;
}

uint32_t tamanio_respuesta_de_crear_segmento_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(resultado_t);
	t += sizeof(direccion);

	return t;
}
// FIN DE PEDIDO DE CREAR SEGMENTO


// COMIENZO DE RESPUESTA DE CREAR SEGMENTO
char* serializar_pedido_interrupcion_t(
	pedido_interrupcion_t* pedido)
{
	char* bytes = malloc(tamanio_pedido_interrupcion_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, serializar_tcb(pedido->tcb), tamanio_tcb_serializado());

	offset += tamanio_tcb_serializado();
	memcpy(bytes + offset, &pedido->direccion_de_memoria, sizeof(direccion));

	return bytes;
}

pedido_interrupcion_t* deserializar_pedido_interrupcion_t(
	char* chorro)
{
	pedido_interrupcion_t* pedido = malloc(sizeof(pedido_interrupcion_t));

	uint32_t offset = 0;
	memcpy(&pedido->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	pedido->tcb = malloc(sizeof(tcb_t));
	memcpy(pedido->tcb, deserializar_tcb(chorro + offset),sizeof(tcb_t));

	offset += sizeof(tcb_t);
	memcpy(&pedido->direccion_de_memoria, chorro + offset, sizeof(direccion));

	return pedido;
}

uint32_t tamanio_pedido_interrupcion_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += tamanio_tcb_serializado();
	t += sizeof(direccion);

	return t;
}
// FIN DE PEDIDO DE INTERRUPCION


// COMIENZO DE PEDIDO DE ENTRADA ESTANDAR

char* serializar_pedido_entrada_estandar_t(
	pedido_entrada_estandar_t* pedido)
{
	char* bytes = malloc(tamanio_pedido_entrada_estandar_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido->pid, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &pedido->tid, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &pedido->identificador_de_tipo, sizeof(idetificador_tipo_t));

	return bytes;
}

pedido_entrada_estandar_t* deserializar_pedido_entrada_estandar_t(
	char* chorro)
{
	pedido_entrada_estandar_t* pedido = malloc(sizeof(pedido_entrada_estandar_t));

	uint32_t offset = 0;
	memcpy(&pedido->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&pedido->pid, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&pedido->tid, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&pedido->identificador_de_tipo, chorro + offset, sizeof(idetificador_tipo_t));

	return pedido;
}

uint32_t tamanio_pedido_entrada_estandar_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(uint32_t);
	t += sizeof(uint32_t);
	t += sizeof(idetificador_tipo_t);

	return t;
}

// FIN DE PEDIDO DE ENTRADA ESTANDAR


// COMIENZO DE PEDIDO DE SALIDA ESTANDAR

char* serializar_pedido_salida_estandar_t(
	pedido_salida_estandar_t* pedido)
{
	char* bytes = malloc(tamanio_pedido_salida_estandar_t_serializado(pedido->tamanio));

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido->pid, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &pedido->tamanio, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, pedido->cadena_de_texto, pedido->tamanio);

	return bytes;
}

pedido_salida_estandar_t* deserializar_pedido_salida_estandar_t(
	char* chorro)
{
	pedido_salida_estandar_t* pedido = malloc(sizeof(pedido_salida_estandar_t));

	uint32_t offset = 0;
	memcpy(&pedido->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&pedido->pid, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&pedido->tamanio, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	pedido->cadena_de_texto = malloc(pedido->tamanio);
	memcpy(pedido->cadena_de_texto, chorro + offset, pedido->tamanio);

	return pedido;
}

uint32_t tamanio_pedido_salida_estandar_t_serializado(uint32_t tamanio)
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(uint32_t);
	t += sizeof(uint32_t);
	t += tamanio;

	return t;
}

// FIN DE PEDIDO DE SALIDA ESTANDAR


// COMIENZO DE PEDIDO DE CREAR HILO

char* serializar_pedido_crear_hilo_t(
	pedido_crear_hilo_t* pedido)
{
	char* bytes = malloc(tamanio_pedido_crear_hilo_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, serializar_tcb(pedido->tcb), tamanio_tcb_serializado());

	return bytes;
}

pedido_crear_hilo_t* deserializar_pedido_crear_hilo_t(
	char* chorro)
{
	pedido_crear_hilo_t* pedido = malloc(
			sizeof(pedido_crear_hilo_t));

	uint32_t offset = 0;
	memcpy(&pedido->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	pedido->tcb = malloc(sizeof(tcb_t));
	memcpy(pedido->tcb, deserializar_tcb(chorro + offset), sizeof(tcb_t));

	return pedido;
}

uint32_t tamanio_pedido_crear_hilo_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += tamanio_tcb_serializado();

	return t;
}

// FIN DE PEDIDO DE CREAR HILO


// COMIENZO DE PEDIDO DE JOIN

char* serializar_pedido_join_t(
	pedido_join_t* pedido)
{
	char* bytes = malloc(tamanio_pedido_join_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido->tid_llamador, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &pedido->tid_esperador, sizeof(uint32_t));

	return bytes;
}

pedido_join_t* deserializar_pedido_join_t(
	char* chorro)
{
	pedido_join_t* pedido = malloc(sizeof(pedido_join_t));

	uint32_t offset = 0;
	memcpy(&pedido->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&pedido->tid_llamador, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&pedido->tid_esperador, chorro + offset, sizeof(uint32_t));

	return pedido;
}

uint32_t tamanio_pedido_join_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(uint32_t);
	t += sizeof(uint32_t);

	return t;
}

// FIN DE PEDIDO DE JOIN


// COMIENZO DE PEDIDO DE BLOQUEAR

char* serializar_pedido_bloquear_t(
	pedido_bloquear_t* pedido)
{
	char* bytes = malloc(tamanio_pedido_bloquear_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, serializar_tcb(pedido->tcb), tamanio_tcb_serializado());

	offset += tamanio_tcb_serializado();
	memcpy(bytes + offset, &pedido->identificador_de_recurso, sizeof(uint32_t));

	return bytes;
}

pedido_bloquear_t* deserializar_pedido_bloquear_t(
	char* chorro)
{
	pedido_bloquear_t* pedido = malloc(sizeof(pedido_bloquear_t));

	uint32_t offset = 0;
	memcpy(&pedido->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	pedido->tcb = malloc(sizeof(tcb_t));
	memcpy(pedido->tcb, deserializar_tcb(chorro + offset), sizeof(tcb_t));

	offset += sizeof(tcb_t);
	memcpy(&pedido->identificador_de_recurso, chorro + offset, sizeof(uint32_t));

	return pedido;
}

uint32_t tamanio_pedido_bloquear_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += tamanio_tcb_serializado();
	t += sizeof(uint32_t);

	return t;
}

// FIN DE PEDIDO DE BLOQUEAR


// COMIENZO DE PEDIDO DE DESPERTAR

char* serializar_pedido_despertar_t(
	pedido_despertar_t* pedido)
{
	char* bytes = malloc(tamanio_pedido_despertar_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido->identificador_de_recurso, sizeof(uint32_t));

	return bytes;
}

pedido_despertar_t* deserializar_pedido_despertar_t(
	char* chorro)
{
	pedido_despertar_t* pedido = malloc(sizeof(pedido_despertar_t));

	uint32_t offset = 0;
	memcpy(&pedido->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&pedido->identificador_de_recurso, chorro + offset, sizeof(uint32_t));

	return pedido;
}

uint32_t tamanio_pedido_despertar_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(uint32_t);

	return t;
}
// FIN DE PEDIDO DE DESPERTAR


// COMIENZO DE RESPUESTA DE ENTRADA ESTANDAR

char* serializar_respuesta_entrada_estandar_t(
	respuesta_entrada_estandar_t* pedido)
{
	char* bytes = malloc(tamanio_respuesta_entrada_estandar_t_serializado(pedido->tamanio));

	uint32_t offset = 0;
	memcpy(bytes + offset, &pedido->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &pedido->tamanio, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &pedido->tid, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, &pedido->pid, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, pedido->cadena, pedido->tamanio);

	return bytes;
}

respuesta_entrada_estandar_t* deserializar_respuesta_entrada_estandar_t(
	char* chorro)
{
	respuesta_entrada_estandar_t* respuesta = malloc(sizeof(respuesta_entrada_estandar_t));

	uint32_t offset = 0;
	memcpy(&respuesta->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&respuesta->tamanio, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&respuesta->pid, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&respuesta->tid, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	respuesta->cadena = malloc(respuesta->tamanio);
	memcpy(respuesta->cadena, chorro + offset, respuesta->tamanio);

	return respuesta;
}

uint32_t tamanio_respuesta_entrada_estandar_t_serializado(uint32_t tamanio)
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(uint32_t);
	t += sizeof(uint32_t);
	t += sizeof(uint32_t);
	t += tamanio;

	return t;
}

// FIN DE RESPUESTA DE ENTRADA ESTANDAR
