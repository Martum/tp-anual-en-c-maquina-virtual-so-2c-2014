#include <stdio.h>
#include <stdlib.h>
#include "sockets.h"
#include <commons/collections/dictionary.h>

int main(int argc, char** argv) {

	t_dictionary* dic_instrucciones = dictionary_create();
	char buffer[4];
	int res = 0;

	printf("Cargo el diccionario de instrucciones (normales y protegidas)\n");

	int memoria = conectar_con_memoria();
	int kernel = conectar_con_kernel();

	if (memoria == -1 || kernel == -1) { // fallo la conexion
		// informo por pantalla y loggeo
		return 0;
	}

	tcb_t *tcb = malloc(sizeof(tcb_t));
	int quantum;
	while (pedir_tcb(tcb, &quantum)); // pide hasta que se lo da

	if (quantum <= 0) {
		// aca paso algo raro porque no deberia mandarte un quantum negativo o igual a 0
	}

	do {
		leer_de_memoria(tcb->pc, sizeof(buffer), buffer);
		int (*funcion)(tcb_t*) = dictionary_get(dic_instrucciones, buffer); // buscar la instruccion en el diccionario
		res = funcion(tcb); // si todo fue como deberia  ser devuelve 0
		quantum--;
	} while (quantum > 0 && res == 0); // si res == 0 significa que la instruccion hizo todas las cosas bien y no termino el proceso

	informar_a_kernel_de_finalizacion(tcb, res);
	printf("Le mando un mensaje al Kernel pasandole el TCBi y la indicacion de porque termine de ejecutar el TCB\n");
	printf("Me quedo esperando a que el Kernel me mande otro TCB\n");

	return 0;
}
