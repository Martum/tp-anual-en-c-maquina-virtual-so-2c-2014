#include "instrucciones.h"
#include <unistd.h>

int32_t main(int32_t argc, char** argv) {

	setvbuf(stdout, NULL, _IONBF, 0); // funcion necesiaria para imprimir en pantalla en eclipse

	if (cargar_configuraciones() == FALLO_CARGA_DE_CONFIGURACIONES) {
		printf("ERROR FALTAL: al cargar configuraciones");
		return 0;
	}

	// TODO descomentar (solamente comentado para pruebas)
	//	if (conectar_con_memoria() == FALLO_CONEXION
	//		|| conectar_con_kernel() == FALLO_CONEXION) {
	//		printf("ERROR FALTAL: Fallo la conexion\n");
	//		return 0;
	//	}

	// TODO eliminar (solamente para pruebas)
	if (conectar_con_memoria() == FALLO_CONEXION) {
		printf("ERROR FATAL FALLO CONEXION CON MEMORIA");
		return 0;
	}

	// TODO eliminar (solamente para pruebas)
	if (conectar_con_kernel() == FALLO_CONEXION) {
		printf("ERROR FATAL FALLO CONEXION CON KERNEL");
		return 0;
	}

	// TODO cambiar a log
	printf("Se pudo conectar a memoria y kernel\n");

	tcb_t tcb;
	t_dictionary* dic_instrucciones = dictionary_create();
	resultado_t (*funcion)(tcb_t*);
	resultado_t resultado = OK;
	int32_t quantum;
	instruccion_t instruccion;

	cargar_diccionario_de_instrucciones(dic_instrucciones);

	// TODO eliminar (solo para pruebas)
	desconectarse();

	// TODO eliminar (solo para pruebas)
	return 0;

	while (1) {
		if (pedir_tcb(&tcb, &quantum) == FALLO_PEDIDO_DE_TCB) {
			printf("ERROR FALTAL: al pedir tcb");
			dictionary_destroy(dic_instrucciones);
			desconectarse();
			return 0;
		}

		// TODO eliminar (no es mas necesario)
		/*
		 dictionary_destroy(dic_instrucciones);
		 cerrar_puertos();
		 printf("ERROR FALTAL: Fallo pedido de tcb\n");
		 return 0;
		 */

		printf("%d\n", quantum); // TODO eliminar (solamente para pruebas)
		printf("%d\n", tcb.a); // TODO eliminar (solamente para pruebas)

		break; // TODO eliminar (solamente para pruebas)

		if ((quantum <= 0) && !tcb.km)
			resultado = ERROR_EN_EJECUCION;

		// TODO eliminar (no es mas necesario)
		/*
		 dictionary_destroy(dic_instrucciones);
		 cerrar_puertos();
		 printf("ERROR FALTAL: Quantum invalido. Aborto\n");
		 break;
		 */

		while ((quantum > 0 || tcb.km) && resultado == OK) {
			sleep(retardo());
			obtener_instruccion(&tcb, instruccion);
			funcion = dictionary_get(dic_instrucciones, instruccion);
			resultado = funcion(&tcb);
			quantum--;
		}

		if (resultado == OK)
			resultado = FIN_QUANTUM;

		if (informar_a_kernel_de_finalizacion(tcb, resultado)
			== FALLO_INFORME_A_KERNEL) {
			printf("ERROR FALTAL: al enviar informe a kernel");
			dictionary_destroy(dic_instrucciones);
			desconectarse();
			return 0;

		}
	}

	dictionary_destroy(dic_instrucciones);

	desconectarse();

	return 0;
}
