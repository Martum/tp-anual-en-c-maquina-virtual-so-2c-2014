/*
 * syscalls.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include <hu4sockets/mensajes.h>

/**
 * Carga las SYSCALLs en memoria.
 * Devuelve 0 en caso de exito; -1 si falla
 */
int32_t cargar_syscalls_a_memoria();

#endif /* SYSCALLS_H_ */
