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

resultado_t _conectar(sock_t** s, char* ip, int32_t puerto) {
	*s = crear_socket_hablador(ip, puerto);
	if (conectar(*s) == -1)
		return FALLO_CONEXION;
	return OK;
}

resultado_t conectar_con_memoria(sock_t** socket) {
	return _conectar(socket, NULL, 4557);
}

resultado_t conectar_con_kernel(sock_t** socket) {
//	return _conectar(socket, NULL, 4559);
	return OK;
}

resultado_t pedir_tcb_compartido(sock_t** kernel, tcb_t* tcb, int32_t* quantum) {
	printf("Me preparo para mandar mensaje\n");
//	pedido_t cuerpo_del_mensaje;
//	cuerpo_del_mensaje.flag = MANDA_TCB;
//	char* mensaje_a_enviar = malloc(sizeof(pedido_t));
//	uint32_t len_a_enviar = sizeof(pedido_t);
//	memcpy(mensaje_a_enviar, &cuerpo_del_mensaje, len_a_enviar);
//	printf("Envio un mensaje de pedido de tcb\n");
//	enviar(*kernel, mensaje_a_enviar, &len_a_enviar);
//	char* mensaje_recibido;
//	uint32_t len_devolucion;
//	recibir(*kernel, &mensaje_recibido, &len_devolucion);
//	respuesta_de_nuevo_tcb_t m_devolucion;
//	memcpy(&m_devolucion, mensaje_recibido, len_devolucion);
//	printf("Quantum: %d\n\n", m_devolucion.quantum);
	tcb->a = 1;
	return OK;
}

direccion crear_segmento(tcb_t* tcb, int32_t bytes) {
	return 0;
}

int32_t destruir_segmento(tcb_t* tcb, direccion direccion) {
	return 0;
}

int32_t leer_de_memoria(direccion dir, int32_t bytes, void* buffer) {
	// tendria que devolver la cantidad de byes que leyo o algo asi
	return 0;
}

int32_t escribir_en_memoria(direccion dir, int32_t byes, void* buffer) {
	// tendria que devolver la cantidad de byes que leyo o algo asi
	return 0;
}

int32_t informar_a_kernel_de_finalizacion(tcb_t* tcb, resultado_t res) {
	// deberia mandar al Kernel el tcb modificado y porque finalizo
	return 0;
}

void obtener_instruccion(tcb_t* tcb, instruccion_t instruccion) {
	leer_de_memoria(tcb->pc, sizeof(instruccion), instruccion);
	tcb->pc = tcb->pc + 4;
}

char obtener_registro(tcb_t* tcb) {
	char* registro = malloc(sizeof(char));
	leer_de_memoria(tcb->pc, sizeof(char), registro);
	tcb->pc = tcb->pc + 1;
	return *registro;
}

int32_t obtener_numero(tcb_t* tcb) {
	int32_t* numero = malloc(sizeof(int32_t));
	leer_de_memoria(tcb->pc, sizeof(int32_t), numero);
	tcb->pc = tcb->pc + 4;
	return *numero;
}
