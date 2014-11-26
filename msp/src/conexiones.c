
#include <stdlib.h>
#include <stdio.h>

#include "configuraciones.h"
#include "conexiones.h"
#include "interfaz.h"
#include "logs.h"

#include <commons/collections/list.h>
#include <commons/string.h>

#include <hu4sockets/mensajes.h>
#include <hu4sockets/sockets.h>

#include <pthread.h>


pthread_mutex_t mutex_conexiones = PTHREAD_MUTEX_INITIALIZER;

fd_set readfds;
int32_t mayor_fd = -1;

void _enviar_flagt(sock_t* conexion, flag_t flag){
	char* respuesta = malloc(tamanio_flagt());
	flag_t resp = flag;
	uint32_t i = tamanio_flagt();

	memcpy(respuesta, &resp, tamanio_flagt());

	enviar(conexion, respuesta, &i);

	free(respuesta);
}


void _recalcular_mayor_fd(int32_t* mayor_fd, int32_t nuevo_fd){
	if(*mayor_fd < nuevo_fd)
		*mayor_fd = nuevo_fd;
}

void* escuchar_conexiones(void* otro_ente){

	sock_t* socket_principal = crear_socket_escuchador(puerto());
	escuchar(socket_principal);

	// Seteamos este como el socket mas grande
	int32_t mayor_fd = socket_principal->fd;
	FD_ZERO(&readfds);
	FD_SET(socket_principal->fd, &readfds);

	// Preparamos el SET
	fd_set readfdset = readfds;

	while(1){
		// Escuchamos el universo
		int rs = select(mayor_fd+1, &readfdset, NULL, NULL, NULL);

		if(rs > 0){

			// Si el socket se puede leer
			if(FD_ISSET(socket_principal->fd, &readfdset)){

				sock_t* nueva_conexion = aceptar_conexion(socket_principal);
				pthread_t hilo_conexion;
				pthread_create(&hilo_conexion, NULL, &_atiendo_hilo_conexion, nueva_conexion);
			}
		}

		// Rearmamos el readfds
		readfdset = readfds;
	}
	return NULL;
}

void* _atiendo_hilo_conexion(void* conexion){

	loggear_trace("Nueva conexion establecida");

	while(1){

		char* msg;
		uint32_t len;
		// Recibimos la identificacion de la conexion
		recibir(conexion, &msg, &len);
		flag_t codop = codigo_operacion(msg);

		switch(codop){

		case CREA_UN_SEGMENTO:
			_atiendo_crear_segmento(conexion,msg);
			break;

		case DESTRUI_SEGMENTO:
			_atiendo_destruir_segmento(conexion, msg);
			break;

		case LEE_DE_MEMORIA:
			_atiendo_leer_memoria(conexion, msg);
			break;

		case ESCRIBI_EN_MEMORIA:
			_atiendo_escribir_memoria(conexion, msg);
			break;

		case DESCONEXION_CPU:
			loggear_trace("Finalizo una conexion");
			pthread_exit(NULL);
			break;

		default:
			break;
		}

	}
}

void _atiendo_crear_segmento(sock_t* sock, char* msg){
	pedido_de_crear_segmento_t* pedido_crear = deserializar_pedido_de_crear_segmento_t(msg);

	loggear_trace("Atiendo solicitud de Crear Segmento.\nPID: %d\nBytes: %d",
			pedido_crear->pid, pedido_crear->tamano);

	resultado_t* resultado = malloc(sizeof(resultado_t));
	direccion dir_base = crear_segmento(pedido_crear->pid, pedido_crear->tamano, resultado);

	respuesta_de_crear_segmento_t* respuesta_crear = malloc(sizeof(respuesta_de_crear_segmento_t));
	respuesta_crear->direccion_virtual = dir_base;
	respuesta_crear->flag = TOMA_SEGMENTO;
	respuesta_crear->resultado = *(resultado);

	free(resultado);

	char* msg_respuesta_crear = serializar_respuesta_de_crear_segmento_t(respuesta_crear);

	uint32_t len_msg_crear = tamanio_respuesta_de_crear_segmento_t_serializado();

	enviar(sock,msg_respuesta_crear, &len_msg_crear);

	free(pedido_crear);
	free(respuesta_crear);
	free(msg_respuesta_crear);
}

void _atiendo_destruir_segmento(sock_t* sock, char* msg){
	pedido_de_destruir_segmento_t* pedido_borrar = deserializar_pedido_de_destruir_segmento_t(msg);

	loggear_trace("Atiendo solicitud de Destruir Segmento.\nPID: %d\nDireccion virtual: %x",
			pedido_borrar->pid, pedido_borrar->direccion_virtual);

	resultado_t* resultado = malloc(sizeof(resultado_t));
	destruir_segmento(pedido_borrar->pid, pedido_borrar->direccion_virtual, resultado);

	respuesta_t * respuesta_borrar = malloc(sizeof(respuesta_t));
	respuesta_borrar->flag = RESPUESTA_DESTRUCCION;
	respuesta_borrar->resultado = *(resultado);

	free(resultado);

	char* msg_respuesta_borrar = serializar_respuesta_t(respuesta_borrar);

	uint32_t len_msg_borrar = tamanio_respuesta_t_serializado();

	enviar(sock,msg_respuesta_borrar, &len_msg_borrar);

	free(pedido_borrar);
	free(respuesta_borrar);
	free(msg_respuesta_borrar);
}

void _atiendo_leer_memoria(sock_t* sock, char* msg){
	pedido_de_leer_de_memoria_t* pedido_leer = deserializar_pedido_de_leer_de_memoria_t(msg);

	loggear_trace("Atiendo solicitud de Leer Memoria.\nPID: %d\nDireccion virtual: %x\n"
			"Tamaño: %d", pedido_leer->pid, pedido_leer->direccion_virtual, pedido_leer->tamano);

	resultado_t* resultado = malloc(sizeof(resultado_t));
	char* bytes = leer_memoria(pedido_leer->pid, pedido_leer->direccion_virtual, pedido_leer->tamano, resultado);

	respuesta_de_leer_de_memoria_t * respuesta_leer = malloc(sizeof(respuesta_de_leer_de_memoria_t));
	respuesta_leer->flag = TOMA_BYTES;
	respuesta_leer->bytes_leido = bytes;
	respuesta_leer->resultado = *(resultado);
	respuesta_leer->tamano = pedido_leer->tamano;

	free(resultado);

	char* msg_respuesta_leer = serializar_respuesta_de_leer_de_memoria_t(respuesta_leer);

	uint32_t len_msg_leer = tamanio_respuesta_de_leer_de_memoria_t_serializado(pedido_leer->tamano);

	enviar(sock,msg_respuesta_leer, &len_msg_leer);

	free(pedido_leer);
	free(respuesta_leer);
	free(msg_respuesta_leer);
}

void _atiendo_escribir_memoria(sock_t* sock, char* msg){
	pedido_de_escribir_en_memoria_t* pedido_escribir = deserializar_pedido_de_escribir_en_memoria_t(msg);

	loggear_trace("Atiendo solicitud de Escribir Memoria.\nPID: %d\nDireccion virtual: %x\n"
			"Bytes: %s\nTamaño: %d", pedido_escribir->pid, pedido_escribir->direccion_virtual,
			pedido_escribir->bytes_a_escribir, pedido_escribir->tamano);

	resultado_t* resultado = malloc(sizeof(resultado_t));
	escribir_memoria(pedido_escribir->pid,
					pedido_escribir->direccion_virtual,
					pedido_escribir->bytes_a_escribir,
					pedido_escribir->tamano,
					resultado);

	respuesta_t* respuesta_escribir = malloc(sizeof(respuesta_t));
	respuesta_escribir->flag = RESPUESTA_ESCRITURA;
	respuesta_escribir->resultado = *(resultado);

	free(resultado);

	char* msg_respuesta_escribir = serializar_respuesta_t(respuesta_escribir);

	uint32_t len_msg_escribir = tamanio_respuesta_t_serializado();

	enviar(sock,msg_respuesta_escribir, &len_msg_escribir);

	free(pedido_escribir);
	free(respuesta_escribir);
	free(msg_respuesta_escribir);
}

