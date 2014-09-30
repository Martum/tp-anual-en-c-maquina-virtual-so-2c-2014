/*
 * memoria.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hu4sockets/mensajes.h"
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
	return OK;
}

void _enviar_y_recibir(sock_t* socket, void* cuerpo_del_mensaje,
		uint32_t tamano, void* m_devolucion) {
	// ARMO EL CHORRO DE BYTES
	char* mensaje_a_enviar = malloc(sizeof(tamano));
	uint32_t len_a_enviar = sizeof(tamano);
	memcpy(mensaje_a_enviar, cuerpo_del_mensaje, len_a_enviar);

	// ENVIO EL MENSAJE
	enviar(socket, mensaje_a_enviar, &len_a_enviar);

	// PREPARO LA RESPUESTA
	char* mensaje_recibido;
	uint32_t len_devolucion;

	// RECIBO LA RESPUESTA
	recibir(socket, &mensaje_recibido, &len_devolucion);

	// ANALIZO LA RESPUESTA
	memcpy(m_devolucion, mensaje_recibido, len_devolucion);
}

resultado_t pedir_tcb(tcb_t* tcb, int32_t* quantum) {

	// ARMO EL MENSAJE
	pedido_t cuerpo_del_mensaje;
	respuesta_de_nuevo_tcb_t m_devolucion;
	cuerpo_del_mensaje.flag = MANDA_TCB;

	// ENVIO Y RECIBO
	_enviar_y_recibir(kernel, &cuerpo_del_mensaje, sizeof(pedido_t),
			&m_devolucion);

	// DESARMO EL MENSAJE
	*tcb = m_devolucion.tcb;
	*quantum = m_devolucion.quantum;

	return OK;
}

direccion crear_segmento(tcb_t* tcb, uint32_t bytes) {
	// ARMO EL MENSAJE
	pedido_de_crear_segmento_t cuerpo_del_mensaje;
	respuesta_de_crear_segmento_t m_devolucion;
	cuerpo_del_mensaje.flag = CREAME_UN_SEGMENTO;
	cuerpo_del_mensaje.pid = tcb->pid;
	cuerpo_del_mensaje.tamano = bytes;

	_enviar_y_recibir(memoria, &cuerpo_del_mensaje,
			sizeof(pedido_de_crear_segmento_t), &m_devolucion);

	return m_devolucion.direccion_virtual;
}

int32_t destruir_segmento(tcb_t* tcb, direccion direccion) {
	// ARMO EL MENSAJE
	pedido_de_destruir_segmento_t cuerpo_del_mensaje;
	respuesta_t m_devolucion;
	cuerpo_del_mensaje.flag = CREAME_UN_SEGMENTO;
	cuerpo_del_mensaje.pid = tcb->pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;

	_enviar_y_recibir(memoria, &cuerpo_del_mensaje,
				sizeof(pedido_de_destruir_segmento_t), &m_devolucion);

	return OK;
}

int32_t leer_de_memoria(tcb_t* tcb, direccion direccion, uint32_t bytes,
		void* buffer) {
	// tendria que devolver la cantidad de byes que leyo o algo asi
	return 0;
}

int32_t escribir_en_memoria(tcb_t* tcb, direccion direccion, uint32_t bytes,
		void* buffer) {
	// tendria que devolver la cantidad de byes que leyo o algo asi
	return 0;
}

int32_t informar_a_kernel_de_finalizacion(tcb_t* tcb, resultado_t res) {
	// deberia mandar al Kernel el tcb modificado y porque finalizo
	return 0;
}

void cerrar_puertos() {
	cerrar_liberar(memoria);
	cerrar_liberar(kernel);
}

void obtener_instruccion(tcb_t* tcb, instruccion_t instruccion) {
	leer_de_memoria(tcb, tcb->pc, sizeof(instruccion), instruccion);
	tcb->pc = tcb->pc + 4;
}

char obtener_registro(tcb_t* tcb) {
	char* registro = malloc(sizeof(char));
	leer_de_memoria(tcb, tcb->pc, sizeof(char), registro);
	tcb->pc = tcb->pc + 1;
	return *registro;
}

int32_t obtener_numero(tcb_t* tcb) {
	int32_t* numero = malloc(sizeof(int32_t));
	leer_de_memoria(tcb, tcb->pc, sizeof(int32_t), numero);
	tcb->pc = tcb->pc + 4;
	return *numero;
}
