/*
 * conexiones_cpu.h
 *
 *  Created on: 16/10/2014
 *      Author: utnso
 */

#ifndef CONEXIONES_CPU_H_
#define CONEXIONES_CPU_H_

#include <hu4sockets/sockets.h>

typedef struct conexion {
	sock_t* socket;
	uint32_t id;
} conexion_t;

void inicializar_lista_conexiones_cpu();
void* escuchar_conexiones(void*);
void _procesar_nueva_conexion(sock_t*, sock_t**);
conexion_t* buscar_conexion_cpu_por_fd(int32_t);
int _atender_socket(conexion_t*);
void _agregar_conexion(sock_t*, uint32_t);

void _informar_te_doy_segmento(sock_t* conexion);
void _informar_respuesta_destruccion_segmento(sock_t* conexion);
void _informar_te_doy_bytes(sock_t* conexion);
void _informar_respuesta_escritura(sock_t* conexion);

void _atiendo_crear_segmento(conexion_t*, char*);
void _atiendo_destruir_segmento(conexion_t*, char*);
void _atiendo_leer_memoria(conexion_t*, char*);
void _atiendo_escribir_memoria(conexion_t*, char*);

#endif /* CONEXIONES_CPU_H_ */
