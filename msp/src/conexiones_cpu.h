/*
 * conexiones_cpu.h
 *
 *  Created on: 16/10/2014
 *      Author: utnso
 */

#ifndef CONEXIONES_CPU_H_
#define CONEXIONES_CPU_H_

typedef struct conexion_cpu {
	sock_t* socket;
	uint32_t id;
} conexion_cpu_t;

void inicializar_lista_conexiones_cpu();
void* escuchar_cpus(void*);

#endif /* CONEXIONES_CPU_H_ */
