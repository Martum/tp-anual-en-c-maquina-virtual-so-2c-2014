/*
 * sockets.h
 *
 *  Created on: 14/9/2014
 *      Author: martin
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define CANTIDAD_CONEXIONES_LISTEN 20

typedef struct sock {
	int32_t fd;
	struct sockaddr_in datos_conexion;

} sock_t;


/**
 * Crea un Socket
 */
sock_t* _crear_socket();

/**
 * Bindea el puerto al Socket
 */
int32_t _bind_puerto(sock_t* socket, uint32_t puerto, char* ip);

/**
 * Bindea el puerto con IP local.
 * Es igual a: _bind_puerto(socket, puerto, "127.0.0.7");
 */
int32_t _bind_puerto_local(sock_t* socket, uint32_t puerto);

/**
 * Conecta con otra PC
 */
int32_t _conectar(sock_t* socket);

/**
 * Establece el socket para escuchar
 */
int32_t _escuchar(sock_t* socket);

/**
 * Acepta una conexion entrante y devuelve el FD
 */
sock_t* _aceptar_conexion(sock_t* socket);

#endif /* SOCKETS_H_ */
