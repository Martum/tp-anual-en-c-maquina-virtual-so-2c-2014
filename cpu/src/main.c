#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "sockets.h"
#include "instrucciones.h"
#include <commons/collections/dictionary.h>
#include "seatest.h"
#include "tests.h"

int32_t main(int32_t argc, char** argv) {

	run_tests(all_tests);

	t_dictionary* dic_instrucciones = dictionary_create();
	tcb_t* tcb = malloc(sizeof(tcb_t));
	char buffer[4];
	int32_t quantum, res = 0;

	cargar_diccionario_de_instrucciones(dic_instrucciones);

	int32_t memoria = conectar_con_memoria();
	int32_t kernel = conectar_con_kernel();

	if (memoria == -1 || kernel == -1) { // fallo la conexion
	// informo por pantalla y loggeo
		return 0;
	}

	while (1) {
		while (pedir_tcb(tcb, &quantum)); // pide hasta que se lo da

		if (quantum < -1 || quantum == 0) {
			// aca paso algo raro porque no deberia mandarte un quantum negativo o igual a 0
		}

		do {
			leer_de_memoria(tcb->pc, sizeof(buffer), buffer);
			int32_t (*funcion)(tcb_t*) = dictionary_get(dic_instrucciones, buffer); // buscar la instruccion en el diccionario
			res = funcion(tcb); // si _todo fue como deberia  ser devuelve 0
			quantum--;
		} while ((quantum > 0 || quantum == -1) && res == 0); // si res == 0 significa que la instruccion hizo todas las cosas bien y no termino el proceso. Quantum -1 significa que es el kernel

		informar_a_kernel_de_finalizacion(tcb, res);
	}

	dictionary_destroy(dic_instrucciones);

	return 0;
}
