#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <commons/collections/dictionary.h>
#include "hu4sockets/sockets.h"
#include "instrucciones.h"
#include "resultados.h"
#include "seatest.h"
#include "sockets.h"
#include "tcb-funciones.h"
#include "tests.h"

int32_t main(int32_t argc, char** argv) {

	run_tests(all_tests);

	sock_t* memoria = NULL;
	sock_t* kernel = NULL;

	if (conectar_con_memoria(memoria) == FALLO_CONEXION
			|| conectar_con_memoria(kernel) == FALLO_CONEXION) { // fallo la conexion
		// informo por pantalla y loggeo
		return 0;
	}

	tcb_t* tcb = crear_tcb();
	t_dictionary* dic_instrucciones = dictionary_create();
	resultado_t (*funcion)(tcb_t*);
	resultado_t res = OK;
	int32_t quantum;
	instruccion_t instruccion;

	cargar_diccionario_de_instrucciones(dic_instrucciones);

	while (1) {
		while (pedir_tcb(tcb, &quantum))
			; // pide hasta que se lo da

		if (quantum < -1 || quantum == 0) {
			// aca paso algo raro porque no deberia mandarte un quantum negativo o igual a 0
		}

		while ((quantum > 0 || quantum == -1) && res == OK) { // Quantum -1 significa que es el kernel
			obtener_instruccion(tcb, instruccion);
			funcion = dictionary_get(dic_instrucciones, instruccion);
			res = funcion(tcb);
			quantum--;
		}

		informar_a_kernel_de_finalizacion(tcb, res);
	}

	dictionary_destroy(dic_instrucciones);
	free(tcb);

	return 0;
}
