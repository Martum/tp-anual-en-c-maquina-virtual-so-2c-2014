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


// FALTA FREE. LO USA SANTI
respuesta_de_nuevo_tcb_t* deserializar_respuesta_de_nuevo_tcb_t(char* chorro)
{
	respuesta_de_nuevo_tcb_t* rta_nuevo_tcb = malloc(sizeof(respuesta_de_nuevo_tcb_t));

	uint32_t offset = 0;
	memcpy(&rta_nuevo_tcb->flag, chorro + offset, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(&rta_nuevo_tcb->quantum, chorro + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&rta_nuevo_tcb->tcb, deserializar_tcb(chorro + offset), sizeof(tcb_t));

	return rta_nuevo_tcb;
}

uint32_t tamanio_respuesta_de_nuevo_tcb_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(uint32_t);
	t += sizeof(tcb_t);

	return t;
}

// LO USO YO. FALTA FREE
char* serializar_respuesta_de_nuevo_tcb_t(respuesta_de_nuevo_tcb_t* rta_nuevo_tcb)
{
	char* bytes = malloc(tamanio_respuesta_de_nuevo_tcb_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes+offset,&rta_nuevo_tcb->flag, sizeof(flag_t));

	offset += sizeof(flag_t);
	memcpy(bytes + offset, &rta_nuevo_tcb->quantum, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(bytes + offset, serializar_tcb(rta_nuevo_tcb->tcb), sizeof(tcb_t));

	return bytes;
}

pedido_t* deserializar_pedido_t(char* chorro){
	pedido_t* pedido = malloc(sizeof(pedido_t));

	uint32_t offset = 0;
	memcpy(&pedido->flag, chorro + offset, sizeof(flag_t));

	return pedido;
}

char* serializar_pedido_t(pedido_t* pedido){
	char* bytes = malloc(tamanio_pedido_t_serializado());

	uint32_t offset = 0;
	memcpy(bytes+offset,&pedido->flag, sizeof(flag_t));

	return bytes;
}

uint32_t tamanio_pedido_t_serializado()
{
	uint32_t t = 0;
	t += sizeof(flag_t);
	t += sizeof(uint32_t);
	t += sizeof(tcb_t);

	return t;
}





