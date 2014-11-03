#include <unistd.h>

#include "commons/error.h"

#include "instrucciones.h"
#include "logs.h"
#include "signals.h"

void _liberar_recursos();

// TODO agregar logs
// TODO cambiar los printf de errores faltes por commons/error

int32_t main(int32_t argc, char** argv)
{
	setvbuf(stdout, NULL, _IONBF, 0); // funcion necesiaria para imprimir en pantalla en eclipse

	empezar_loggeo();

	escuchar_signals();

	if (cargar_configuraciones() == FALLO_CARGA_DE_CONFIGURACIONES)
	{
		loggear_error("No se pudieron cargar las configuraciones");
		error_show("ERROR FALTAL: al cargar configuraciones");
		return 0;
	}

	// TODO eliminar (solamente comentado para pruebas)
	//	if (conectar_con_memoria() == FALLO_CONEXION
	//		|| conectar_con_kernel() == FALLO_CONEXION) {
	//		printf("ERROR FALTAL: Fallo la conexion\n");
	//		return 0;
	//	}

	if (conectar_con_memoria() == FALLO_CONEXION)
	{
		loggear_error("No pudo conectarse con memoria");
		liberar_configuraciones();
		finalizar_loggeo();
		error_show(" Al tratar de conectarse con memoria");
		return 0;
	}

	// TODO descomentar (solamente comentado para pruebas)
//	if (conectar_con_kernel() == FALLO_CONEXION)
//	{
//		loggear_error("No pudo conectarse con memoria");
//		finalizar_loggeo();
//		error_show(" Al tratar de conectarse con kernel");
//		return 0;
//	}

	// TODO eliminar (solo para pruebas)
	direccion direccion;
	crear_segmento(12, 123, &direccion);
	printf("Direccion: %d\n", direccion);

	// TODO eliminar (solo para pruebas)
	char bytes = 'a';
	escribir_en_memoria(12, direccion, 1, &bytes);

	// TODO eliminar (solo para pruebas)
	_liberar_recursos();

	// TODO eliminar (solo para pruebas)
	return 0;

	tcb_t tcb;
	resultado_t (*funcion)(tcb_t*);
	resultado_t resultado = OK;
	int32_t quantum;
	instruccion_t instruccion;

	inicializar_dic_de_instrucciones();

	loggear_trace("Cargadas todas las estructuras administrativas");

	while (1)
	{
		if (pedir_tcb(&tcb, &quantum) == FALLO_PEDIDO_DE_TCB)
		{
			printf("ERROR FALTAL: al pedir tcb");
			_liberar_recursos();
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

			if (leer_proxima_instruccion(&tcb, instruccion)
				== FALLO_LECTURA_DE_MEMORIA)
				resultado = ERROR_EN_EJECUCION;

			obtener_funcion_segun_instruccion(funcion, instruccion);

			resultado = funcion(&tcb);

			quantum--;
		}

		if (resultado == OK)
			resultado = FIN_QUANTUM;

		if (informar_a_kernel_de_finalizacion(tcb, resultado)
			== FALLO_INFORME_A_KERNEL)
		{
			printf("ERROR FALTAL: al enviar informe a kernel");
			_liberar_recursos();
			return 0;
		}
	}

	_liberar_recursos();

	return 0;
}

void _liberar_recursos()
{
	liberar_configuraciones();
	liberar_dic_de_instrucciones();
	desconectarse();
	finalizar_loggeo();
}
