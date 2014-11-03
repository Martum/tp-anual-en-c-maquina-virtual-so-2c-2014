#include <unistd.h>

#include "instrucciones.h"
#include "signals.h"

int32_t main(int32_t argc, char** argv)
{
//	setvbuf(stdout, NULL, _IONBF, 0); // funcion necesiaria para imprimir en pantalla en eclipse

	escuchar_signals();

	sleep(1000);

	if (cargar_configuraciones() == FALLO_CARGA_DE_CONFIGURACIONES)
	{
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
	if (conectar_con_memoria() == FALLO_CONEXION)
	{
		printf("ERROR FATAL: al conectarse con memoria");
		return 0;
	}

	// TODO eliminar (solamente para pruebas)
//	if (conectar_con_kernel() == FALLO_CONEXION)
//	{
//		printf("ERROR FATAL: al conectarse con kernel");
//		return 0;
//	}

	// TODO cambiar a log
	printf("Se pudo conectar a memoria y kernel\n");

	direccion direccion;
	crear_segmento(12, 123, &direccion);
	printf("Direccion: %d\n", direccion);

	char bytes = 'a';
	escribir_en_memoria(12, direccion, 1, &bytes);

	tcb_t tcb;
	resultado_t (*funcion)(tcb_t*);
	resultado_t resultado = OK;
	int32_t quantum;
	instruccion_t instruccion;

	inicializar_dic_de_instrucciones();

	// TODO eliminar (solo para pruebas)
	desconectarse();

	// TODO eliminar (solo para pruebas)
	return 0;

	while (1)
	{
		if (pedir_tcb(&tcb, &quantum) == FALLO_PEDIDO_DE_TCB)
		{
			printf("ERROR FALTAL: al pedir tcb");
			liberar_dic_de_instrucciones();
			desconectarse();
			return 0;
		}

		printf("%d\n", quantum); // TODO eliminar (solamente para pruebas)
		printf("%d\n", tcb.a); // TODO eliminar (solamente para pruebas)

		break; // TODO eliminar (solamente para pruebas)

		if ((quantum <= 0) && !tcb.km)
			resultado = ERROR_EN_EJECUCION;

		while ((quantum > 0 || tcb.km) && resultado == OK)
		{
			sleep(retardo());

			if (obtener_instruccion(&tcb, instruccion)
				== FALLO_LECTURA_DE_MEMORIA)
				resultado = ERROR_EN_EJECUCION;

			obtener_funcion(funcion, instruccion);

			resultado = funcion(&tcb);

			quantum--;
		}

		if (resultado == OK)
			resultado = FIN_QUANTUM;

		if (informar_a_kernel_de_finalizacion(tcb, resultado)
			== FALLO_INFORME_A_KERNEL)
		{
			printf("ERROR FALTAL: al enviar informe a kernel");
			liberar_dic_de_instrucciones();
			desconectarse();
			return 0;
		}
	}

	liberar_dic_de_instrucciones();
	desconectarse();

	return 0;
}
