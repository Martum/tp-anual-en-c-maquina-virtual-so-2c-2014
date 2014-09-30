/*
 * memoria.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include "sockets.h"

sock_t* memoria;
sock_t* kernel;

resultado_t _conectar(sock_t** socket, char* ip, int32_t puerto) {
	*socket = crear_socket_hablador(ip, puerto);
	if (conectar(*socket) == -1)
		return FALLO_CONEXION;
	return OK;
}

resultado_t conectar_con_memoria() {
	return _conectar(&memoria, NULL, 4557);
}

resultado_t conectar_con_kernel() {
	return _conectar(&kernel, NULL, 4559);
}

resultado_t _enviar_y_recibir(sock_t* socket, void* cuerpo_del_mensaje,
		uint32_t tamano, void* m_devolucion) {
	// ARMO EL CHORRO DE BYTES
	char* mensaje_a_enviar = malloc(sizeof(tamano));
	uint32_t len_a_enviar = sizeof(tamano);
	memcpy(mensaje_a_enviar, cuerpo_del_mensaje, len_a_enviar);

	// ENVIO EL MENSAJE
	if (enviar(socket, mensaje_a_enviar, &len_a_enviar) == -1)
		return FALLO_COMUNICACION;

	// PREPARO LA RESPUESTA
	char* mensaje_recibido;
	uint32_t len_devolucion;

	// RECIBO LA RESPUESTA
	if (recibir(socket, &mensaje_recibido, &len_devolucion) == -1)
		return FALLO_COMUNICACION;

	// ANALIZO LA RESPUESTA
	memcpy(m_devolucion, mensaje_recibido, len_devolucion);

	return OK;
}

resultado_t pedir_tcb(tcb_t* tcb, int32_t* quantum) {

	// ARMO EL MENSAJE
	pedido_t cuerpo_del_mensaje;
	respuesta_de_nuevo_tcb_t m_devolucion;
	cuerpo_del_mensaje.flag = MANDA_TCB;

	// ENVIO Y RECIBO

	if (_enviar_y_recibir(kernel, &cuerpo_del_mensaje, sizeof(pedido_t),
			&m_devolucion) == FALLO_COMUNICACION) {
		return FALLO_PEDIDO_DE_TCB;
	}

	// DESARMO EL MENSAJE
	*tcb = m_devolucion.tcb;
	*quantum = m_devolucion.quantum;

	return OK;
}

resultado_t crear_segmento(direccion pid, uint32_t bytes, direccion* direccion) {
	// ARMO EL MENSAJE
	pedido_de_crear_segmento_t cuerpo_del_mensaje;
	respuesta_de_crear_segmento_t m_devolucion;
	cuerpo_del_mensaje.flag = CREAME_UN_SEGMENTO;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.tamano = bytes;

	if (_enviar_y_recibir(memoria, &cuerpo_del_mensaje,
			sizeof(pedido_de_crear_segmento_t), &m_devolucion)
			== FALLO_COMUNICACION) {
		return FALLO_CREACION_DE_SEGMENTO;
	}

	*direccion = m_devolucion.direccion_virtual;

	return OK;
}

resultado_t destruir_segmento(direccion pid, direccion direccion) {
	// ARMO EL MENSAJE
	pedido_de_destruir_segmento_t cuerpo_del_mensaje;
	respuesta_t m_devolucion;
	cuerpo_del_mensaje.flag = DESTRUI_SEGMENTO;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;

	return _enviar_y_recibir(memoria, &cuerpo_del_mensaje,
			sizeof(pedido_de_destruir_segmento_t), &m_devolucion);
}

resultado_t leer_de_memoria(direccion pid, direccion direccion, uint32_t bytes,
		void* buffer) {
	pedido_de_leer_de_memoria_t cuerpo_del_mensaje;
	respuesta_de_leer_de_memoria_t m_devolucion;
	cuerpo_del_mensaje.flag = LEE_DE_MEMORIA;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;
	cuerpo_del_mensaje.tamano = bytes;

	if (_enviar_y_recibir(memoria, &cuerpo_del_mensaje,
			sizeof(pedido_de_leer_de_memoria_t), &m_devolucion))
		return FALLO_LECTURA_DE_MEMORIA;

	memcpy(buffer, m_devolucion.bytes_leido, sizeof(m_devolucion.bytes_leido));

	return OK;
}

resultado_t escribir_en_memoria(direccion pid, direccion direccion, uint32_t bytes,
		void* buffer) {
	pedido_de_escribir_en_memoria_t cuerpo_del_mensaje;
	respuesta_t m_devolucion;
	cuerpo_del_mensaje.flag = ESCRIBI_EN_MEMORIA;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;
	cuerpo_del_mensaje.bytes_a_escribir = (char*) buffer;
	cuerpo_del_mensaje.tamano = bytes;

	return _enviar_y_recibir(memoria, &cuerpo_del_mensaje,
			sizeof(pedido_de_escribir_en_memoria_t), &m_devolucion);
}

int32_t informar_a_kernel_de_finalizacion(tcb_t tcb, resultado_t res) {
	pedido_con_resultado_t cuerpo_del_mensaje;
	respuesta_t m_devolucion;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.tcb = tcb;
	cuerpo_del_mensaje.resultado = res;

	return _enviar_y_recibir(kernel, &cuerpo_del_mensaje,
			sizeof(pedido_con_resultado_t), &m_devolucion);
}

void cerrar_puertos() {
	cerrar_liberar(memoria);
	cerrar_liberar(kernel);
}

void obtener_instruccion(tcb_t tcb, instruccion_t* instruccion) {
	leer_de_memoria(tcb.pid, tcb.pc, sizeof(instruccion), instruccion);
	tcb.pc = tcb.pc + 4;
}

void obtener_registro(tcb_t tcb, char* registro) {
	leer_de_memoria(tcb.pid, tcb.pc, sizeof(char), registro);
	tcb.pc = tcb.pc + 1;
}

void obtener_numero(tcb_t tcb, int32_t* numero) {
	leer_de_memoria(tcb.pid, tcb.pc, sizeof(int32_t), numero);
	tcb.pc = tcb.pc + 4;
}
