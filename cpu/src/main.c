#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "sockets.h"
#include "instrucciones.h"
#include <commons/collections/dictionary.h>
#include "seatest.h"
#include "tests.h"
#include "resultados.h"

int32_t main(int32_t argc, char** argv) {

	run_tests(all_tests);

	t_dictionary* dic_instrucciones = dictionary_create();
	tcb_t* tcb = crear_tcb();
	resultado_t res = OK;
	int32_t quantum;
	char buffer[4];

	cargar_diccionario_de_instrucciones(dic_instrucciones);

	int32_t memoria = conectar_con_memoria();
	int32_t kernel = conectar_con_kernel();

	if (memoria == FALLO_CONEXION || kernel == FALLO_CONEXION) { // fallo la conexion
	// informo por pantalla y loggeo
		return 0;
	}

	while (1) {
		while (pedir_tcb(tcb, &quantum)); // pide hasta que se lo da

		if (quantum < -1 || quantum == 0) {
			// aca paso algo raro porque no deberia mandarte un quantum negativo o igual a 0
		}

		while ((quantum > 0 || quantum == -1) && res == OK) { // si res == 0 significa que la instruccion hizo todas las cosas bien y no termino el proceso. Quantum -1 significa que es el kernel
			leer_de_memoria(tcb->pc, sizeof(buffer), buffer);
			resultado_t (*funcion)(tcb_t*) = dictionary_get(dic_instrucciones, buffer); // buscar la instruccion en el diccionario
			res = funcion(tcb); // si _todo fue como deberia  ser devuelve 0
			quantum--;
		}

		informar_a_kernel_de_finalizacion(tcb, res);
	}

	dictionary_destroy(dic_instrucciones);
	free(tcb);
	free(buffer);

	return 0;
}
