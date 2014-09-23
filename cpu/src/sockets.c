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
#include "hu4sockets/sockets.h"
#include "tcb-estructuras.h"
#include "instrucciones-estructuras.h"
#include "sockets-estructuras.h"
#include "resultados.h"

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
	return _conectar(socket, NULL, 4559);
}

void _armar_mensaje(char** mensaje, uint32_t* len) {
	*mensaje = malloc(sizeof(mensaje_aviso_t));
	*len = sizeof(mensaje_aviso_t);
	mensaje_aviso_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = MANDA_TCB;
	memcpy(*mensaje, &cuerpo_del_mensaje, sizeof(mensaje_aviso_t));
}

void __enviar(sock_t** kernel, char* msg, uint32_t len) {
	printf("Envio\n");
	enviar(*kernel, msg, &len);
}

void __recibir(sock_t** kernel) {
	printf("Me preparo para recibir\n");
	char* msg;
	uint32_t len;
	printf("Recibo\n");
	recibir(*kernel, &msg, &len);
	mensaje_tcb_t m_devolucion;
	memcpy(&m_devolucion, msg, len);
	printf("Quantum: %d\n\n", m_devolucion.quantum);
}

resultado_t pedir_tcb(sock_t** kernel, tcb_t* tcb, int32_t* quantum) {
	printf("Me preparo para mandar\n");
	char* mensaje_a_enviar;
	uint32_t len_a_enviar;
	_armar_mensaje(&mensaje_a_enviar, &len_a_enviar);
	__enviar(kernel, mensaje_a_enviar, len_a_enviar);
	__recibir(kernel);
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
