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
#define CABECERA_VALIDA 666

typedef struct sock
{
	int32_t fd;
	struct sockaddr_in* datos_conexion;

} sock_t;


typedef struct cabecera
{
	uint32_t longitud_mensaje;
	uint16_t valido;
} cabecera_t;


/**
 * Crea un Socket
 */
sock_t* _crear_socket();

/**
 * Prepara la conexion para trabajar con el socket.
 * Si es localhost usar NULL en la IP
 */
void _preparar_conexion(sock_t* socket, char* ip, uint32_t puerto);

/**
 * Crea un socket y lo prepara para conectarse
 */
sock_t* _crear_y_preparar(char* ip, uint32_t puerto);

/**
 * Limpia el puerto para reutilizarlo (para los casos en que se corre
 * varias veces un bind sobre el mismo puerto)
 */
int32_t _limpiar_puerto(sock_t* socket);

/**
 * Bindea el puerto al Socket
 */
int32_t _bind_puerto(sock_t* socket);

/**
 * Envia un mensaje a traves del socket
 * Devuelve la cantidad de bytes que se enviaron realmente, o -1 en caso de error
 */
int32_t _enviar(sock_t* socket, char* msg, uint32_t len);


/**
 * Trata de enviar todo el mensaje, aunque este sea muy grande.
 * Deja en len la cantidad de bytes NO enviados
 * Devuelve 0 en caso de exito, o -1 si falla
 */
int32_t _enviar_todo(sock_t* socket, char* msg, uint32_t* len);

/**
 * Lee un mensaje y lo almacena en buff
 * Devuelve la cantidad leia de bytes, 0 en caso de que la conexion este cerrada, o -1 en caso de error
 */
int32_t _recibir(sock_t* socket, char* buff, uint32_t buf_max_size);

/**
 * Trata de recibir todo el mensaje, aunque este se envie en varias rafagas.
 * Deja en len la cantidad de bytes NO recibidos
 * Devuelve 0 en caso de exito; o -1 si falla
 */
int32_t _recibir_todo(sock_t* socket, char* buff, uint32_t* len);

/**
 * Le pedimos a Nestor que cierre el socket
 */
void _cerrame_esto_nestor(sock_t* socket);

/**
 * Liberamos la memoria ocupada por el struct
 */
void _liberar_memoria(sock_t* socket);

/**
 * Devuelve el tamanio de la cabecera.
 * Es sinonimo de sizeof(cabecera_t)
 */
uint32_t _tamanio_cabecera();

/**
 * Crea una cabecera vacia
 */
cabecera_t* _crear_cabecera_vacia();

/**
 * Crea una cabecera para enviar datos
 */
cabecera_t* _crear_cabecera(uint32_t len);

/**
 * Serializa la cabecera para enviarla por la Matrix
 */
char* _serializar_cabecera(cabecera_t* cabecera);

/**
 * Crea una cabecera y la serializa
 */
char* _crear_cabecera_serializada(uint32_t len);

/**
 * Deserializar cabecera.
 * Devuelve 0 en caso de exito; o -1 si no se recibio bien
 */
int32_t _deserealizar_cabecera(cabecera_t* cabecera, char* bytes);

/**
 * Recibir cabecera
 * Devuelve 0 en caso de exito; o -1 si no se recibio bien
 */
int32_t _recibir_cabecera(sock_t* socket, cabecera_t* cabecera);



/***FUNCIONES PBLICAS***/

/**
 * Crea un socket y para escuchar conexiones y lo bindea al puerto especificado.
 * Puerto es el puerto en el que se va a escuchar nuevas conexiones.
 * NO realiza el listen.
 *
 * @RETURNS: El struct socket. Si el puerto esta ocupado retorna -1
 */
sock_t* crear_socket_escuchador(uint32_t puerto);

/**
 * Crea un socket para enviar mensajes.
 * Puerto es el puerto de la maquina remota que esta escuchando.
 * Si queres especificar el puerto de salida de tu PC la tenes atroden.
 * NO realiza el connect.
 *
 * @RETURNS: El struct socket.
 */
sock_t* crear_socket_hablador(char* ip, uint32_t puerto);

/**
 * Conecta con otra PC
 *
 * @RETURNS: -1 en caso de error
 * NOTA: Recordar que es una funcion bloqueante
 */
int32_t conectar(sock_t* socket);

/**
 * Establece el socket para escuchar
 * TODO: Esta funcion podria incluirse en crear_socket_escuchador ya que no es bloqueante
 *
 * @RETURNS: -1 en caso de error
 */
int32_t escuchar(sock_t* socket);

/**
 * Acepta una conexion entrante
 *
 * @RETURNS: el nuevo FD; o NULL en caso de error
 * NOTA: Recordar que es una funcion bloqueante
 */
sock_t* aceptar_conexion(sock_t* socket);

/**
 * Envia el msg. Len es la longitud del chorro de bytes a enviar.
 *
 * @RETURNS: 0 en caso de exito; o -1 si falla y deja en len la cantidad de bytes no enviados
 */
int32_t enviar(sock_t* socket, char* msg, uint32_t* len);

/**
 * Recibe el msg. Len es la longitud del chorro de bytes recibidos.
 *
 * @RETURNS: 0 en caso de exito; o -1 si falla y deja en len la cantidad de bytes leidos correctamente
 * NOTA: En caso de fallar el buffer queda ocupando el tamanio completo del mensaje, no solo el leido
 * 		 Una vez procesados (o descartados) los bytes recibidos, limpiar la memoria
 */
int32_t recibir(sock_t* socket, char** msg, uint32_t* len);

/**
 * Cierra el Socket y libera la memoria ocupada por el Struct
 */
void cerrar_liberar(sock_t* socket);

#endif /* SOCKETS_H_ */
