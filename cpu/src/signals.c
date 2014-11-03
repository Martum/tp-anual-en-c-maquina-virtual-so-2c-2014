/*
 * signals.c
 *
 *  Created on: 03/11/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <signal.h>

#include "signals.h"

void catch(int sig)
{
	printf("Señal: %d atrapada!\n", sig);
	exit(0);
}

void escuchar_signals()
{
	signal(SIGINT, & catch);
}
