/*
 * signals.c
 *
 *  Created on: 03/11/2014
 *      Author: utnso
 */

#include "signals.h"

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
