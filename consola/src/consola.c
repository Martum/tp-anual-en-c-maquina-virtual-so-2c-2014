/*
 * consola.c
 *
 *  Created on: 12/10/2014
 *      Author: martin
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hu4sockets/sockets.h>
#include <hu4sockets/mensajes.h>
#include <signal.h>
#include <pthread.h>

#include "configuraciones.h"

typedef enum {
	BOK,
	BSIN_MEMORIA,	// Sin memoria para el programa
	BOTROS_ERRORES,
	BNO_SE_ENVIO_CODIGO,
	BMENSAJE_INCOMPLETO

} errores_t;

sock_t* SOCKET_KERNEL;
uint32_t entrada_en_progreso = 0;
uint32_t entrada_estandar_pid = 0;
uint32_t entrada_estandar_tid = 0;

/**
 * Carga el codigo BESO a memoria
 * @RETURNS: Puntero al codigo BESO
 */
char* cargar_beso(char* path, uint32_t* len);

/**
 * Envia el codigo BESO al Kernel
 */
errores_t enviar_beso_al_kernel(char* codigo_beso, uint32_t size);


/**
 * Escucha por conexiones del KERNEL.
 */
void escuchar_kernel();

/**
 * Escucha las signals de desconexion
 */
void escuchar_seniales();

int main(int argc, char **argv)
{
	// Verificamos codigo BESO
	if(argc != 2)
	{
		printf("Debe indicar la ruta del codigo BESO");
		return -1;
	}

	// Levantamos archivo de configuraciones
	cargar_configuraciones();

	// Cargamos el codigo BESO
	uint32_t len;
	char* codigo_beso = cargar_beso(argv[1], &len);
	if(codigo_beso == NULL)
	{
		printf("Ruta incorrecta al codigo BESO");
		return -2;
	}

	int32_t salida = 0;
	errores_t e = enviar_beso_al_kernel(codigo_beso, len);

	switch(e)
	{
		case BSIN_MEMORIA:
			printf("No hay memoria disponible");
			salida = -3;
			break;

		case BOTROS_ERRORES:
			printf("Ocurrio un error desconocido");
			salida = -4;
			break;

		case BNO_SE_ENVIO_CODIGO:
			printf("No se pudo enviar el codigo");
			salida = -5;
			break;

		case BMENSAJE_INCOMPLETO:
			printf("No se recibio el mensaje completamente");
			salida = -6; //TODO: Aca convendria reenviar...
			break;

		case BOK:
			free(codigo_beso);
			escuchar_seniales();
			escuchar_kernel();
			return 0;
			break;
	}

	free(codigo_beso);
	cerrar_liberar(SOCKET_KERNEL);
	return salida;
}

/**
 * Carga el Codigo Beso en memoria.
 */
char* cargar_beso(char* path, uint32_t* len)
{
	FILE* archivo = fopen(path, "r");

	// No existe el archivo
	if(archivo == NULL)
		return NULL;

	// Obtenemos el size del archivo
	fseek(archivo, 0L, SEEK_END);
	//*len = ftell(archivo) - 1;	//TODO: Linea de siempre
	*len = ftell(archivo);
	fseek(archivo, 0L, SEEK_SET);

	// Alocamos espacio suficiente
	char* codigo = malloc(*len);

	// Leemos el archivo
	fread(codigo, sizeof(char), *len, archivo);

	return codigo;
}

errores_t enviar_beso_al_kernel(char* codigo_beso, uint32_t size)
{
	SOCKET_KERNEL = crear_socket_hablador(ip_kernel(), puerto_kernel());
	conectar(SOCKET_KERNEL);

	// Enviamos mensaje de identificacion
	char* mensaje = malloc(tamanio_flagt());
	uint32_t len = tamanio_flagt();
	flag_t f = SOY_PROGRAMA;
	memcpy(mensaje, &f, tamanio_flagt());

	enviar(SOCKET_KERNEL, mensaje, &len);
	free(mensaje);

	// Recibimos la bienvenida
	recibir(SOCKET_KERNEL, &mensaje, &len);
	memcpy(&f, mensaje, tamanio_flagt());

	if(f != BIENVENIDO)
	{// Si no somos bienvenidos
		free(mensaje);
		return BOTROS_ERRORES;
	}

	// Enviamos el codigo BESO
	if(enviar(SOCKET_KERNEL, codigo_beso, &size) == -1)
	{
		free(mensaje);
		return BNO_SE_ENVIO_CODIGO;
	}

	// Recibimos la respuesta
	recibir(SOCKET_KERNEL, &mensaje, &len);
	memcpy(&f, mensaje, tamanio_flagt());

	errores_t salida;
	switch(f)
	{
		case SIN_MEMORIA:
			salida = BSIN_MEMORIA;
			break;

		case MENSAJE_INCOMPLETO:
			salida = BMENSAJE_INCOMPLETO;
			break;

		case RECIBIDO_OK:
			salida = BOK;
			break;

		default:
			break;
	}

	free(mensaje);
	return salida;
}

/**
 * Muestra al usuario lo indicado por el Kernel.
 */
void salida_estandar(pedido_salida_estandar_t* salida)
{
	if(salida->identificador_de_tipo == CADENA)
	{
		char* texto = malloc(salida->tamanio + 1);
		memcpy(texto, salida->cadena_de_texto, salida->tamanio);
		texto[salida->tamanio] = '\0';

		printf("SALIDA > %s\n", texto);

		free(texto);
	}
	else
	{
		int32_t entero;
		memcpy(&entero, salida->cadena_de_texto, sizeof(int32_t));
		printf("SALIDA > %d\n", entero);
	}

	free(salida->cadena_de_texto);
	free(salida);
}

/**
 * Pide y procesa la entrada del dato correspondiente.
 *
 * @RETURNS: Un respuesta_entrada_estandar_t* con la informacion cargada
 */
respuesta_entrada_estandar_t* entrada_estandar(pedido_entrada_estandar_t* entrada)
{
	respuesta_entrada_estandar_t* respuesta_entrada = malloc(sizeof(respuesta_entrada_estandar_t));
	respuesta_entrada->flag = RESPUESTA_ENTRADA;
	respuesta_entrada->resultado = COMPLETADO_OK;
	respuesta_entrada->pid = entrada->pid;
	respuesta_entrada->tid = entrada->tid;

	entrada_estandar_pid = entrada->pid;
	entrada_estandar_tid = entrada->tid;

	// TODO: Deberiamos verificar que el tipo de entrada sea el correcto.
	if(entrada->identificador_de_tipo == ENTERO)
	{
		int32_t* entero = malloc(sizeof(int32_t));
		printf("ENTRADA (entero) > ");
		scanf("%d", entero);

		respuesta_entrada->tamanio = sizeof(int32_t);
		respuesta_entrada->cadena = malloc(sizeof(int32_t));
		memcpy(respuesta_entrada->cadena, entero, sizeof(int32_t));

		free(entero);
	}
	else
	{
		char* texto = malloc(51);
		printf("ENTRADA (cadena) > ");
		scanf("%50[^\n]", texto);

		respuesta_entrada->tamanio = strlen(texto);
		respuesta_entrada->cadena = texto;
	}

	free(entrada);
	return respuesta_entrada;
}

/**
 * Envia al Kernel la respuesta a la entrada solicitada.
 */
void enviar_respuesta_entrada(respuesta_entrada_estandar_t* respuesta_entrada)
{
	char* msj = serializar_respuesta_entrada_estandar_t(respuesta_entrada);
	uint32_t i = tamanio_respuesta_entrada_estandar_t_serializado(respuesta_entrada->tamanio);

	enviar(SOCKET_KERNEL, msj, &i);

	free(respuesta_entrada->cadena);
	free(respuesta_entrada);
	free(msj);
}

void procesar_conexion(char* mensaje, uint32_t len)
{
	flag_t codop = codigo_operacion(mensaje);

	switch(codop)
	{
		case ENTRADA_ESTANDAR:
			entrada_en_progreso = 1;
			enviar_respuesta_entrada(entrada_estandar(deserializar_pedido_entrada_estandar_t(mensaje)));
			entrada_en_progreso = 0;
			break;

		case SALIDA_ESTANDAR:
			salida_estandar(deserializar_pedido_salida_estandar_t(mensaje));
			break;

		case TERMINAR_CONEXION:
			cerrar_liberar(SOCKET_KERNEL);
			printf("- Se cierra el proceso (KERNEL REQ - ter)");
			exit(-1);
			break;

		default:
			cerrar_liberar(SOCKET_KERNEL);
			printf("- Se cierra el proceso (KERNEL REQ - def)");
			exit(-1);
			break;
	}
}

void _enviar_flagt(sock_t* conxion, flag_t flag)
{
	char* msg = malloc(tamanio_flagt());
	uint32_t i = tamanio_flagt();

	memcpy(msg, &flag, tamanio_flagt());

	enviar(conxion, msg, &i);

	free(msg);
}

void notificar_desconexion_kernel()
{
	// Si entrada en progreso == 1 -> notificar que no se va a completar
	// Desconectarse
	if(entrada_en_progreso == 1)
	{
		respuesta_entrada_estandar_t* respuesta_entrada = malloc(sizeof(respuesta_entrada_estandar_t));
		respuesta_entrada->flag = RESPUESTA_ENTRADA;
		respuesta_entrada->resultado = FALLO_ENTRADA_ESTANDAR;
		respuesta_entrada->pid = entrada_estandar_pid;
		respuesta_entrada->tid = entrada_estandar_tid;

		// Para que no quede vacio
		respuesta_entrada->tamanio = 1;
		respuesta_entrada->cadena = malloc(2);
		strcpy(respuesta_entrada->cadena, "a");

		enviar_respuesta_entrada(respuesta_entrada);
	}

	_enviar_flagt(SOCKET_KERNEL, TERMINAR_CONEXION);

	cerrar_liberar(SOCKET_KERNEL);
	printf("- Se cierra el proceso (SIGINT)");
	exit(-1);
}

void escuchar_seniales()
{
	signal(SIGINT, notificar_desconexion_kernel);
	//signal(SIGUSR1, fallo_memoria);
}

void escuchar_kernel()
{
	char* mensaje;
	uint32_t len;
	while(1)
	{
		if(recibir(SOCKET_KERNEL, &mensaje, &len) == 0)
		{
			procesar_conexion(mensaje, len);
		}
		else
		{
			cerrar_liberar(SOCKET_KERNEL);
			printf("- Se cierra el proceso (KERNEL REQ - nop)");
			exit(-1);
		}

		free(mensaje);
	}
}
