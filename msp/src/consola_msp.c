/*
 * consola_msp.c
 *
 *  Created on: 17/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdbool.h>

#include <commons/collections/list.h>
#include <commons/bitarray.h>

#include "interfaz.h"
#include "consola_msp.h"
#include "configuraciones.h"

int main(void){
	cargar_configuraciones();
	inicializar_lista_procesos();
	inicializar_lista_marcos();
	inicializar_memoria_fisica_total();
	return 0;
}
