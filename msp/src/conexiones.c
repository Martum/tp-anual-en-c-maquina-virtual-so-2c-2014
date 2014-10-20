
#include <stdlib.h>
#include <stdio.h>

#include "configuraciones.h"
#include "conexiones.h"
#include "interfaz.h"

#include <commons/collections/list.h>
#include <commons/string.h>

#include <hu4sockets/mensajes.h>
#include <hu4sockets/sockets.h>

#include <pthread.h>


pthread_mutex_t mutex_conexiones = PTHREAD_MUTEX_INITIALIZER;
t_list* lista_conexiones;
fd_set readfds;

void inicializar_lista_conexiones_cpu(){
	lista_conexiones = list_create();
}

void _agregar_conexion(sock_t* conexion_socket, uint32_t id){

	conexion_t* conexion = malloc(sizeof(conexion_t));
	conexion->socket = conexion_socket;
	conexion->id = id;

	pthread_mutex_lock(&mutex_conexiones);

	list_add(lista_conexiones, conexion);
	FD_SET(conexion->socket->fd, &readfds);

	pthread_mutex_unlock(&mutex_conexiones);
}

void _enviar_flagt(sock_t* conexion, flag_t flag){
	char* respuesta = malloc(tamanio_flagt());
	flag_t resp = flag;
	uint32_t i = tamanio_flagt();

	memcpy(respuesta, &resp, tamanio_flagt());

	enviar(conexion, respuesta, &i);

	free(respuesta);
}

void _informar_te_doy_segmento(sock_t* conexion){
	_enviar_flagt(conexion, TOMA_SEGMENTO);
}

void _informar_respuesta_destruccion_segmento(sock_t* conexion){
	_enviar_flagt(conexion, RESPUESTA_DESTRUCCION);
}

void _informar_te_doy_bytes(sock_t* conexion){
	_enviar_flagt(conexion, TOMA_BYTES);
}

void _informar_respuesta_escritura(sock_t* conexion){
	_enviar_flagt(conexion, RESPUESTA_ESCRITURA);
}

conexion_t* buscar_conexion_por_fd(int32_t fd){

	// Funcion de busqueda
	bool buscar_conex(void* elemento){
		conexion_t* conexion = (conexion_t*) elemento;
		return (conexion->socket->fd) == fd;
	}

	// Buscamos...
	pthread_mutex_lock(&mutex_conexiones);
	conexion_t* conexion = list_find(lista_conexiones, buscar_conex);
	pthread_mutex_unlock(&mutex_conexiones);

	return conexion;
}

void* escuchar_conexiones(void* otro_ente){

	sock_t* principal = crear_socket_escuchador(puerto());
	escuchar(principal);

	// Seteamos este como el socket mas grande
	int32_t mayor_fd = principal->fd;
	FD_ZERO(&readfds);
	FD_SET(principal->fd, &readfds);

	// Preparamos el SET
	fd_set readfdset = readfds;

	while(1){
		// Escuchamos el universo
		int rs = select(mayor_fd+1, &readfdset, NULL, NULL, NULL);

		if(rs > 0){

			// Vemos si hay sockets para leer
			int32_t i;
			int32_t copia_mayor_fd = mayor_fd;

			for(i = 0; i < (copia_mayor_fd+1); i++){

				// Si el socket se puede leer
				if(FD_ISSET(i, &readfdset)){

					if(i == principal->fd){

						// Es el socket principal, new connection knocking
						sock_t* nueva_conexion;
						nueva_conexion = aceptar_conexion(principal);
						_procesar_nueva_conexion(nueva_conexion);

					}else{

						// No es el socket principal, es un proceso
						// int tipo_msg =
								_atender_socket(buscar_conexion_por_fd(i));

					}
				}
			}
		}

		// Rearmamos el readfds
		readfdset = readfds;
	}
	return NULL;
}

void _procesar_nueva_conexion(sock_t* nueva_conexion){
	conexion_t* ultima_conex = (conexion_t*)list_take(lista_conexiones, list_size(lista_conexiones));
	_agregar_conexion(nueva_conexion, ultima_conex->id + 1);
}

int _atender_socket(conexion_t* conexion){
	char* msg;
	uint32_t len;
	// Recibimos la identificacion de la conexion
	recibir(conexion->socket, &msg, &len);
	flag_t codop = codigo_operacion(msg);

	int salida = 0;

	switch(codop){

		case CREAME_UN_SEGMENTO:
			salida = 1;

			pedido_de_crear_segmento_t* pedido_crear = deserializar_pedido_de_crear_segmento_t(msg);

			direccion dir_base = crear_segmento(pedido_crear->pid, pedido_crear->tamano);

			respuesta_de_crear_segmento_t* respuesta_crear = malloc(sizeof(respuesta_de_crear_segmento_t));
			respuesta_crear->direccion_virtual = dir_base;
			// respuesta->resultado = ...
			respuesta_crear->flag = TOMA_SEGMENTO;

/*						,

						RESPUESTA_ESCRITURA
						*/
			char* msg_respuesta_crear = serializar_respuesta_de_crear_segmento_t(respuesta_crear);

			uint32_t* len_msg_crear = malloc(sizeof(uint32_t));
			*(len_msg_crear) = tamanio_respuesta_de_crear_segmento_t_serializado();

			enviar(conexion->socket,msg_respuesta_crear, len_msg_crear);

			free(len_msg_crear);
			free(pedido_crear);
			free(respuesta_crear);
			free(msg_respuesta_crear);

			break;

		case DESTRUI_SEGMENTO:
			salida = 2;

			pedido_de_destruir_segmento_t* pedido_borrar = deserializar_pedido_de_destruir_segmento_t(msg);

			destruir_segmento(pedido_borrar->pid, pedido_borrar->direccion_virtual);

			respuesta_t * respuesta_borrar = malloc(sizeof(respuesta_t));
			respuesta_borrar->flag = RESPUESTA_DESTRUCCION;
			// respuesta_borrar->resultado = ...
			char* msg_respuesta_borrar = serializar_respuesta_t(respuesta_borrar);

			uint32_t* len_msg_borrar = malloc(sizeof(uint32_t));
			*(len_msg_borrar) = tamanio_respuesta_t_serializado();

			enviar(conexion->socket,msg_respuesta_borrar, len_msg_borrar);

			free(len_msg_borrar);
			free(pedido_borrar);
			free(respuesta_borrar);
			free(msg_respuesta_borrar);

			break;

		case LEE_DE_MEMORIA:
			salida = 3;

			pedido_de_leer_de_memoria_t* pedido_leer = deserializar_pedido_de_leer_de_memoria_t(msg);

			char* bytes = leer_memoria(pedido_leer->pid, pedido_leer->direccion_virtual, pedido_leer->tamano);

			respuesta_de_leer_de_memoria_t * respuesta_leer = malloc(sizeof(respuesta_de_leer_de_memoria_t));
			respuesta_leer->flag = TOMA_BYTES;
			respuesta_leer->bytes_leido = bytes;
			// respuesta_leer->resultado = ...
			// respuesta_leer->tamano = (uint32_t)string_length(bytes);

			char* msg_respuesta_leer = serializar_respuesta_de_leer_de_memoria_t(respuesta_leer);

			uint32_t* len_msg_leer = malloc(sizeof(uint32_t));
			*(len_msg_leer) = tamanio_respuesta_de_leer_de_memoria_t_serializado();

			enviar(conexion->socket,msg_respuesta_leer, len_msg_leer);

			free(len_msg_leer);
			free(pedido_leer);
			free(respuesta_leer);
			free(msg_respuesta_leer);
			break;

		case ESCRIBI_EN_MEMORIA:
			salida = 4;
/*
			pedido_de_crear_segmento_t* pedido = deserializar_pedido_de_crear_segmento_t(msg);

			direccion dir_base = crear_segmento(pedido->pid, pedido->tamano);

			respuesta_de_crear_segmento_t* respuesta = malloc(sizeof(respuesta_de_crear_segmento_t));
			respuesta->direccion_virtual = dir_base;
			// respuesta->resultado = ...
			// respuesta->flag = ...
			char* msg_respuesta = serializar_respuesta_de_crear_segmento_t(respuesta);

			enviar(conexion->socket,msg_respuesta, tamanio_respuesta_de_crear_segmento_t_serializado());
			free(respuesta);
			free(msg_respuesta);
*/
			break;

		default:
			break;
	}
	return salida;
}
