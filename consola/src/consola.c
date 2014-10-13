/*
 * consola.c
 *
 *  Created on: 12/10/2014
 *      Author: martin
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configuraciones.h"

typedef enum {
	OK,
	SIN_MEMORIA,	// Sin memoria para el programa

} errores_t;

/**
 * Carga el codigo BESO a memoria
 * @RETURNS: Puntero al codigo BESO
 */
char* cargar_beso(char* path);
errores_t enviar_beso_al_kernel(char* codigo_beso);

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
	char* codigo_beso = cargar_beso(argv[1]);
	if(codigo_beso == NULL)
	{
		printf("Ruta incorrecta al codigo BESO");
		return -2;
	}

	errores_t e = enviar_beso_al_kernel(codigo_beso);

	switch(e)
	{
		case SIN_MEMORIA:
			printf("No hay memoria disponible");
			return -3;
			break;

		case OK:
			// TODO: Creamos THREAD para escuchar al kernel
			break;
	}
}

char* cargar_beso(char* path)
{
	FILE* archivo = fopen(path, "r");

	// No existe el archivo
	if(archivo == NULL)
		return NULL;

	// Obtenemos el size del archivo
	fseek(archivo, 0L, SEEK_END);
	uint32_t size = ftell(archivo);
	fseek(archivo, 0L, SEEK_SET);

	// Alocamos espacio suficiente
	char* codigo = malloc(size);
	uint32_t len = 0;

	// TODO: Seguir aca. Mirar el link de favoritos.
}
