/*
 * sockets.c
 *
 *  Created on: 16/9/2014
 *      Author: martin
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sockets.h"


sock_t* _crear_socket()
{
	sock_t* sock = malloc(sizeof(sock_t));
	sock->datos_conexion = malloc(sizeof(struct sockaddr));

	sock->fd = socket(AF_INET, SOCK_STREAM, 0);

	return sock;
}


int32_t _bind_puerto(sock_t* socket, uint32_t puerto, char* ip)
{
	// Seteamos la IP, si es NULL es localhost
	if(ip != NULL)
		socket->datos_conexion->sin_addr.s_addr = htonl(INADDR_ANY); // Local
	else
		socket->datos_conexion->sin_addr.s_addr = inet_addr(ip);	// Remota

	socket->datos_conexion->sin_family = AF_INET;
	socket->datos_conexion->sin_port = htons(puerto);
	memset(&(socket->datos_conexion->sin_zero), '\0', 8);

	return bind(socket->fd, (struct sockaddr *)&socket->datos_conexion, sizeof(struct sockaddr));
}


int32_t _bind_puerto_local(sock_t* socket, uint32_t puerto)
{
	return _bind_puerto(socket, puerto, NULL);
}


int32_t _conectar(sock_t* socket)
{
	return connect(socket->fd, (struct sockaddr *)&socket->datos_conexion, sizeof(struct sockaddr));
}


int32_t _escuchar(sock_t* socket)
{
	return listen(socket->fd, CANTIDAD_CONEXIONES_LISTEN);
}


sock_t* _aceptar_conexion(sock_t* socket)
{
	sock_t* sock_nuevo = _crear_socket();
	uint32_t i = sizeof(struct sockaddr_in);	// TODO: Ver que hacer con este INT. Sirve devolverlo dentro del struct?

	sock_nuevo->fd = accept(socket->fd, (struct sockaddr *)&sock_nuevo->datos_conexion, &i);

	return sock_nuevo;
}
