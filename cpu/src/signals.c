/*
 * signals.c
 *
 *  Created on: 03/11/2014
 *      Author: utnso
 */

#include "signals.h"

#include <stdio.h>
#include <signal.h>

#include "configuraciones.h"
#include "instrucciones.h"
#include "logs.h"
#include "sockets.h"


void atrapar_interrupcion(int sig)
{
	loggear_warning("Atrapada interrupcion");
	liberar_configuraciones();
	liberar_dic_de_instrucciones();
	desconectarse();
	finalizar_loggeo();
	exit(0);
}

void escuchar_signals()
{
	signal(SIGINT, atrapar_interrupcion);
}
