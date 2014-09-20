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

typedef struct sock
{
	int32_t fd;
	struct sockaddr_in* datos_conexion;

} sock_t;


typedef struct cabecera
{
	uint32_t longitud_mensaje;
} cabecera_t;


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
 * Devuelve -1 en caso de error
 */
int32_t _conectar(sock_t* socket);

/**
 * Establece el socket para escuchar
 * Devuelve -1 en caso de error
 */
int32_t _escuchar(sock_t* socket);

/**
 * Acepta una conexion entrante
 * Devuelve el nuevo FD
 */
sock_t* _aceptar_conexion(sock_t* socket);

/**
 * Envia un mensaje a traves del socket
 * Devuelve la cantidad de bytes que se enviaron realmente, o -1 en caso de error
 */
uint32_t _enviar(sock_t* socket, char* msg, uint32_t len);


/**
 * Trata de enviar todo el mensaje, aunque este sea muy grande.
 * Deja en len la cantidad de bytes NO enviados
 * Devuelve 0 en caso de exito, o -1 si falla
 */
uint32_t _enviar_todo(sock_t* socket, char* msg, uint32_t* len);

/**
 * Lee un mensaje y lo almacena en buff
 * Devuelve la cantidad leia de bytes, 0 en caso de que la conexion este cerrada, o -1 en caso de error
 */
uint32_t _recibir(sock_t* socket, char* buff, uint32_t buf_max_size);

/**
 * Le pedimos a Nestor que cierre el socket
 */
void _cerrame_esto_nestor(sock_t* socket);

/**
 * Liberamos la memoria ocupada por el struct
 */
void _liberar_memoria(sock_t* socket);

/**
 * Serializa la cabecera para enviarla por la Matrix
 */
char* _serializar_cabecera(cabecera_t* cabecera);



/***FUNCIONES PBLICAS***/

/**
 * Envia el msg. Len es la longitud del chorro de bytes.
 * Devuelve 0 en caso de exito; o -1 si falla y deja en len la cantidad de bytes no enviados
 */
uint32_t enviar(sock_t* socket, char* msg, uint32_t* len);

/**
 * Cierra el Socket y libera la memoria ocupada por el Struct
 */
void cerrar_liberar(sock_t* socket);

#endif /* SOCKETS_H_ */
