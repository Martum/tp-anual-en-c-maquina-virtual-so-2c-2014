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


uint32_t _enviar(sock_t* socket, char* msg, uint32_t len)
{
	return send(socket->fd, msg, len, 0);
}

uint32_t _enviar_todo(sock_t* socket, char* msg, uint32_t* len)
{
	int32_t total = 0;        // cuantos bytes hemos enviado
	int32_t bytesleft = *len; // cuantos se han quedado pendientes
	int32_t n;
	while(total < *len) {
		n = _enviar(socket, msg+total, bytesleft);

		// Si falla el envio cortamos el loop
		if (n == -1)
			break;

		total += n;
		bytesleft -= n;
	}

	*len = total; // devuelve aqui la cantidad enviada en realidad
	return n==-1?-1:0; // devuelve -1 si hay fallo, 0 en otro caso
}


uint32_t _recibir(sock_t* socket, char* buff, uint32_t buf_max_size)
{
	return recv(socket->fd, buff, buf_max_size, 0);
}


void _cerrame_esto_nestor(sock_t* socket)
{
	shutdown(socket->fd, 2);
}


void _liberar_memoria(sock_t* socket)
{
	free(socket->datos_conexion);
	free(socket);
}

char* _serializar_cabecera(cabecera_t* cabecera)
{
	char* msg = malloc(sizeof(cabecera_t));
	memcpy(msg, cabecera, sizeof(cabecera_t));

	return msg;
}


uint32_t enviar(sock_t* socket, char* msg, uint32_t* len)
{
	// Seteamos el Header
	uint32_t len_cabecera = sizeof(cabecera_t);
	cabecera_t cabecera;
	cabecera.longitud_mensaje = *len;

	// Enviamos el Header, si falla devolvemos -1
	if(_enviar_todo(socket, _serializar_cabecera(&cabecera), &len_cabecera) != 0)
		return -1;

	// Enviamos el Mensaje, si falla devolvemos -1
	if(_enviar_todo(socket, msg, len))
		return -1;

	return 0;
}


void cerrar_liberar(sock_t* socket)
{
	_cerrame_esto_nestor(socket);
	_liberar_memoria(socket);
}
