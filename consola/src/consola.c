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
#include <pthread.h>

#include "configuraciones.h"

typedef enum {
	BOK,
	BSIN_MEMORIA,	// Sin memoria para el programa
	BOTROS_ERRORES,
	BNO_SE_ENVIO_CODIGO,
	BMENSAJE_INCOMPLETO

} errores_t;

sock_t* socket_kernel;

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
			escuchar_kernel();
			return 0;
			break;
	}

	free(codigo_beso);
	cerrar_liberar(socket_kernel);
	return salida;
}

char* cargar_beso(char* path, uint32_t* len)
{
	FILE* archivo = fopen(path, "r");

	// No existe el archivo
	if(archivo == NULL)
		return NULL;

	// Obtenemos el size del archivo
	fseek(archivo, 0L, SEEK_END);
	*len = ftell(archivo) - 1;
	fseek(archivo, 0L, SEEK_SET);

	// Alocamos espacio suficiente
	char* codigo = malloc(*len);

	// Leemos el archivo
	fread(codigo, sizeof(char), *len, archivo);

	return codigo;
}

errores_t enviar_beso_al_kernel(char* codigo_beso, uint32_t size)
{
	socket_kernel = crear_socket_hablador(ip_kernel(), puerto_kernel());
	conectar(socket_kernel);

	// Enviamos mensaje de identificacion
	char* mensaje = malloc(tamanio_flagt());
	uint32_t len = tamanio_flagt();
	flag_t f = SOY_PROGRAMA;
	memcpy(mensaje, &f, tamanio_flagt());

	enviar(socket_kernel, mensaje, &len);
	free(mensaje);

	// Recibimos la bienvenida
	recibir(socket_kernel, &mensaje, &len);
	memcpy(&f, mensaje, tamanio_flagt());

	if(f != BIENVENIDO)
	{// Si no somos bienvenidos
		free(mensaje);
		return BOTROS_ERRORES;
	}

	// Enviamos el codigo BESO
	if(enviar(socket_kernel, codigo_beso, &size) == -1)
	{
		free(mensaje);
		return BNO_SE_ENVIO_CODIGO;
	}

	// Recibimos la respuesta
	recibir(socket_kernel, &mensaje, &len);
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

void escuchar_kernel()
{

	while(1)
	{

	}
}
