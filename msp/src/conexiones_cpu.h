/*
 * conexiones_cpu.h
 *
 *  Created on: 16/10/2014
 *      Author: utnso
 */

#ifndef CONEXIONES_CPU_H_
#define CONEXIONES_CPU_H_

#include <hu4sockets/sockets.h>

typedef struct conexion_cpu {
	sock_t* socket;
	uint32_t id;
} conexion_cpu_t;

void inicializar_lista_conexiones_cpu();
void* escuchar_cpus(void*);
int32_t _procesar_nueva_conexion(sock_t*, sock_t*);
sock_t* buscar_conexion_cpu_por_fd(int32_t fd);

void _informar_te_doy_segmento(sock_t* conexion);
void _informar_respuesta_destruccion_segmento(sock_t* conexion);
void _informar_te_doy_bytes(sock_t* conexion);
void _informar_respuesta_escritura(sock_t* conexion);

#endif /* CONEXIONES_CPU_H_ */
