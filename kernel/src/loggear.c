/*
 * loggear.c
 *
 *  Created on: 28/11/2014
 *      Author: martin
 */

#include "loggear.h"
#include "configuraciones.h"
#include <ansisop-panel/panel/panel.h>
#include <ansisop-panel/panel/kernel.h>
#include <hu4sockets/tcb.h>
#include <stdlib.h>
#include <commons/collections/list.h>

void cargar_loggeo()
{
	inicializar_panel(KERNEL, ruta_logeo());
}

t_hilo* crear_t_hilo(tcb_t* tcb)
{
	t_hilo* hilot = malloc(sizeof(t_hilo));

	hilot->pid = tcb->pid;
	hilot->tid = tcb->tid;

	return hilot;
}

void logear_instruccion_protegida(char* instruccion, tcb_t* tcb)
{
	t_hilo* hilot = crear_t_hilo(tcb);

	instruccion_protegida(instruccion, hilot);

	free(hilot);
}

void loggear_conexion_consola(uint32_t pid)
{
	conexion_consola(pid);
}

void loggear_desconexion_consola(uint32_t pid)
{
	desconexion_consola(pid);
}

void loggear_conexion_cpu(uint32_t id)
{
	conexion_cpu(id);
}

void loggear_desconexion_cpu(uint32_t id)
{
	desconexion_cpu(id);
}

void loggear_estado_de_hilos()
{
	//t_list* lista_tcbs = get_todos_los_tcbs();
}
