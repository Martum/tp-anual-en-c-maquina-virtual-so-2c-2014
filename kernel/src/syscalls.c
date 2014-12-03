/*
 * syscalls.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include "syscall.h"
#include <hu4sockets/mensajes.h>
#include "lstestados.h"
#include "memoria.h"
#include <hu4sockets/resultados.h>
#include "configuraciones.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* cargar_syscalls(char* path, uint32_t* len)
{
	FILE* archivo = fopen(path, "r");

	// No existe el archivo
	if(archivo == NULL)
		return NULL;

	// Obtenemos el size del archivo
	fseek(archivo, 0L, SEEK_END);
	*len = ftell(archivo);
	fseek(archivo, 0L, SEEK_SET);

	// Alocamos espacio suficiente
	char* codigo = malloc(*len);

	// Leemos el archivo
	fread(codigo, sizeof(char), *len, archivo);

	return codigo;
}

int32_t cargar_syscalls_a_memoria(char* path)
{
	uint32_t len;
	char* syscalls = cargar_syscalls(path, &len);

	direccion direccion;

	if(crear_segmento(1, len, &direccion) != 0 || escribir_memoria(1, direccion, syscalls, len) != OK)
		return -1;

	free(syscalls);

	if(crear_segmento(1, tamanio_stack(), &direccion) != 0)
		return -1;

	tcb_t* tcbkm = get_tcb_km();
	tcbkm->base_stack = direccion;
	tcbkm->cursor_stack = direccion;

	return 0;
}
