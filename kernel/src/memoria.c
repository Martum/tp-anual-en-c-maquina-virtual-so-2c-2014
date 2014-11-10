/*
 * memoria.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */
#include "memoria.h"

/*int32_t crear_segmento(uint32_t pid, uint32_t tamanio, direccion* direccion)
{
	return -1;
}

void destruir_segmento(uint32_t pid, direccion base_segmento)
{

}

int32_t escribir_memoria(uint32_t pid, direccion base_segmento, char* chorro_bytes, uint32_t len)
{
	return -1;
}*/

sock_t* memoria;

void conectar_memoria()
{
	// TODO: Codificar esto
}

resultado_t _enviar_y_recibir(sock_t* socket, char* chorro_a_enviar,
	uint32_t len_a_enviar, char* respuesta)
{
	loggear_trace("Me preparo para enviar %d bytes", len_a_enviar);

	if (enviar(socket, chorro_a_enviar, &len_a_enviar) == -1)
	{
		loggear_warning("No pudo mandar el paquete de %d bytes", len_a_enviar);
		return FALLO_COMUNICACION;
	}

	loggear_trace("Envio con exito paquete de %d bytes", len_a_enviar);

	char* mensaje_recibido;
	uint32_t len_devolucion;

	loggear_trace("Me preparo para recibir respuesta");

	if (recibir(socket, &mensaje_recibido, &len_devolucion) == -1)
	{
		loggear_warning("No pudo recibir respuesta");
		return FALLO_COMUNICACION;
	}

	loggear_trace("Recibio con exito paquete de %d bytes", len_devolucion);

	memcpy(respuesta, mensaje_recibido, len_devolucion);

	return OK;
}

resultado_t crear_segmento(direccion pid, uint32_t tamanio,
	direccion* direccion)
{
	loggear_trace("Preparando mensaje para crear segmento");

	pedido_de_crear_segmento_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = CREA_UN_SEGMENTO;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.tamano = tamanio;

	char* chorro_de_envio = serializar_pedido_de_crear_segmento_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(
		tamanio_respuesta_de_crear_segmento_t_serializado());

	loggear_trace("PID %d", pid);
	loggear_trace("Tamanio del segmento %d", tamanio);

	if (_enviar_y_recibir(memoria, chorro_de_envio,
		tamanio_pedido_de_crear_segmento_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return -1;
	}

	loggear_trace("Recibi respuesta por creacion de segmento");

	respuesta_de_crear_segmento_t* respuesta =
		deserializar_respuesta_de_crear_segmento_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->resultado == ERROR_DE_MEMORIA_LLENA)
	{
		loggear_warning(
			"La creacion no pudo realizarse porque la memoria estaba llena");
		free(respuesta);
		return -1;
	}

	*direccion = respuesta->direccion_virtual;

	free(respuesta);

	loggear_debug("Creacion de segmento satisfactoria");

	loggear_trace("Direccion del segmento %d", *direccion);

	return 0;
}

resultado_t destruir_segmento(direccion pid, direccion direccion)
{
	loggear_trace("Preparando mensaje para destruir segmento");

	pedido_de_destruir_segmento_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = DESTRUI_SEGMENTO;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;

	char* chorro_de_envio = serializar_pedido_de_destruir_segmento_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_respuesta_t_serializado());

	loggear_trace("PID: %d", pid);
	loggear_trace("Direccion %d", direccion);

	if (_enviar_y_recibir(memoria, chorro_de_envio,
		tamanio_pedido_de_destruir_segmento_t_serializado(),
		chorro_de_respuesta) == FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_DESTRUCCION_DE_SEGMENTO;
	}

	loggear_trace("Recibi respuesta por destruccion de segmento");

	respuesta_t* respuesta = deserializar_respuesta_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->resultado == ERROR_NO_ENCUENTRO_SEGMENTO)
	{
		loggear_warning(
			"La destruccion no pudo realizarse por no encontrar el segmetno");
		free(respuesta);
		return FALLO_DESTRUCCION_DE_SEGMENTO;
	}

	free(respuesta);

	loggear_debug("Destruccion de segmento satisfactoria");

	return OK;
}

resultado_t leer_de_memoria(direccion pid, direccion direccion,
	uint32_t cantidad_de_bytes, char* buffer)
{
	loggear_trace("Preparando mensaje para leer de memoria");

	pedido_de_leer_de_memoria_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = LEE_DE_MEMORIA;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;
	cuerpo_del_mensaje.tamano = cantidad_de_bytes;

	char* chorro_de_envio = serializar_pedido_de_leer_de_memoria_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(
		tamanio_respuesta_de_leer_de_memoria_t_serializado(cantidad_de_bytes));

	loggear_trace("Cantidad de bytes a leer %d", cantidad_de_bytes);

	if (_enviar_y_recibir(memoria, chorro_de_envio,
		tamanio_pedido_de_leer_de_memoria_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_LECTURA_DE_MEMORIA;
	}

	loggear_trace("Recibi respuesta por lectura de memoria");

	respuesta_de_leer_de_memoria_t* respuesta =
		deserializar_respuesta_de_leer_de_memoria_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->resultado == SEGMENTATION_FAULT)
	{
		loggear_warning(
			"La lectura no pudo realizarse por violacion de segmento");
//		free(respuesta->bytes_leido);
		free(respuesta);
		return FALLO_LECTURA_DE_MEMORIA;
	}

	memcpy(buffer, respuesta->bytes_leido, respuesta->tamano);

//	free(respuesta->bytes_leido);
	free(respuesta);

	loggear_debug("Lectura de memorira satisfactoria");

	loggear_trace("Cantidad de bytes leidos %d", respuesta->tamano);

	return OK;
}

resultado_t escribir_en_memoria(direccion pid, direccion direccion, char* bytes_a_escribir,
		uint32_t cantidad_de_bytes)
{
	loggear_trace("Preparando mensaje escribir en memoria");

	pedido_de_escribir_en_memoria_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = ESCRIBI_EN_MEMORIA;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;
	cuerpo_del_mensaje.bytes_a_escribir = bytes_a_escribir;
	cuerpo_del_mensaje.tamano = cantidad_de_bytes;

	char* chorro_de_envio = serializar_pedido_de_escribir_en_memoria_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_respuesta_t_serializado());

	loggear_trace("Cantidad de bytes a escribir %d", cantidad_de_bytes);
	loggear_trace("Bytes %s", bytes_a_escribir);

	if (_enviar_y_recibir(memoria, chorro_de_envio,
		tamanio_pedido_de_escribir_en_memoria_t_serializado(cantidad_de_bytes),
		chorro_de_respuesta) == FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_ESCRITURA_EN_MEMORIA;
	}

	loggear_trace("Recibi respuesta por escritura en memoria");

	respuesta_t* respuesta = deserializar_respuesta_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->resultado == SEGMENTATION_FAULT)
	{
		loggear_warning(
			"La escritura no pudo realizarse por violacion de segmento");
		free(respuesta);
		return FALLO_ESCRITURA_EN_MEMORIA;
	}

	free(respuesta);

	loggear_debug("Escritura en memoria satisfactoria");

	return OK;
}
