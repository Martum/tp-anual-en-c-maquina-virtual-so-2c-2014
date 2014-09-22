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
	inicializar_lista_procesos();
	inicializar_memoria_fisica_total(10000);// el parametro viene por archivo de configuracion
	return 0;
}
