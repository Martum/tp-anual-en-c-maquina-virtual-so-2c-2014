#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <commons/collections/dictionary.h>
#include "instrucciones.h"
#include "sockets.h"

int32_t main(int32_t argc, char** argv) {

	setvbuf(stdout, NULL, _IONBF, 0); // funcion necesiaria para imprimir en pantalla en eclipse

	sock_t* memoria;
	sock_t* kernel;

	if (conectar_con_memoria(&memoria) == FALLO_CONEXION
			|| conectar_con_kernel(&kernel) == FALLO_CONEXION) { // todo informar por pantalla y log
		printf("Fallo la conexion. Aborto\n");
		return 0;
	}

	printf("Se pudo conectar a memoria y kernel");

	tcb_t tcb;
	t_dictionary* dic_instrucciones = dictionary_create();
	resultado_t (*funcion)(tcb_t*);
	resultado_t res = OK;
	int32_t quantum;
	instruccion_t instruccion;

	cargar_diccionario_de_instrucciones(dic_instrucciones);

	while (1) {
		pedir_tcb(&kernel, &tcb, &quantum);
		break;

		if (quantum < -1 || quantum == 0) {
			printf("Quantum invalido. Aborto\n");
			break;
		} // aca paso algo raro porque no deberia mandarte un quantum negativo o igual a 0

		while ((quantum > 0 || quantum == -1) && res == OK) { // Quantum -1 significa que es el kernel
			obtener_instruccion(&tcb, instruccion);
			funcion = dictionary_get(dic_instrucciones, instruccion);
			res = funcion(&tcb);
			quantum--;
		}

		informar_a_kernel_de_finalizacion(&tcb, res);
	}

	dictionary_destroy(dic_instrucciones);

	cerrar_liberar(memoria);
	cerrar_liberar(kernel);

	return 0;
}
