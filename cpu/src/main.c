/*
 * main.c
 *
 *  Created on: 03/11/2014
 *      Author: utnso
 */

#include <unistd.h>

#include "commons/error.h"

#include "instrucciones.h"
#include "logs.h"
#include "signals.h"

void _liberar_recursos();

// TODO agregar logs
// TODO cambiar los printf de errores faltes por commons/error
// TODO ver que pasa cuando hay ERROR_EN_EJECUCION

int32_t main(int32_t argc, char** argv)
{
//	setvbuf(stdout, NULL, _IONBF, 0); // funcion necesiaria para imprimir en pantalla en eclipse

	empezar_loggeo();

	escuchar_signals();

	if (cargar_configuraciones() == FALLO_CARGA_DE_CONFIGURACIONES)
	{
		loggear_error("No pudo cargar las configuraciones");
		loggear_info("Liberando recursos para cierre...");
		finalizar_loggeo();
		error_show(" Al cargar configuraciones");
		return 0;
	}

	if (conectar_con_memoria() == FALLO_CONEXION)
	{
		loggear_error("No pudo conectarse con memoria");
		loggear_info("Liberando recursos para cierre...");
		liberar_configuraciones();
		finalizar_loggeo();
		error_show(" Al tratar de conectarse con memoria");
		return 0;
	}

	// TODO descomentar (solamente comentado para pruebas)
//	if (conectar_con_kernel() == FALLO_CONEXION)
//	{
//		loggear_error("No pudo conectarse con memoria");
//		loggear_info("Liberando recursos para cierre...");
//		liberar_configuraciones();
//		finalizar_loggeo();
//		error_show(" Al tratar de conectarse con kernel");
//		return 0;
//	}

	// TODO eliminar (solo para pruebas)
	direccion direccion;
	crear_segmento(123, 50, &direccion);


	// TODO eliminar (solo para pruebas)
	char* bytes = "santiago perez torre";
	int tam = strlen(bytes);
	escribir_en_memoria(123, direccion, tam, bytes);

	// TODO eliminar (solo para pruebas)
	char* buffer = malloc(tam);
	leer_de_memoria(123, direccion, tam, buffer);

	// TODO eliminar (solo para pruebas)
	destruir_segmento(123, direccion);

	instruccion_t instruccion;
	int32_t quantum;
	resultado_t (*funcion)(tcb_t*);
	resultado_t resultado = OK;
	tcb_t tcb;

	inicializar_dic_de_instrucciones();

	loggear_info("Cargadas todas las estructuras administrativas");

	// TODO eliminar (solo para pruebas)
	_liberar_recursos();

	// TODO eliminar (solo para pruebas)
	return 0;

	while (1)
	{
		if (pedir_tcb(&tcb, &quantum) == FALLO_PEDIDO_DE_TCB)
		{
			loggear_error("No pudo pedir tcb a kernel");
			_liberar_recursos();
			error_show(" Al pedir tcb a kernel");
			return 0;
		}

		if ((quantum <= 0) && !tcb.km)
		{
			loggear_warning("Se recibio un quantum %d menor a 0", quantum);
			resultado = ERROR_EN_EJECUCION;
		}

		while ((quantum > 0 || tcb.km) && resultado == OK)
		{
			loggear_trace("Me preparo para ejecutar otra instruccion.");
			loggear_trace("Quantum restante", quantum);
			loggear_trace("Modo kernel %d", tcb.km);

			sleep(retardo());

			if (leer_proxima_instruccion(&tcb, instruccion)
				== FALLO_LECTURA_DE_MEMORIA)
			{
				resultado = ERROR_EN_EJECUCION;
			}

			obtener_funcion_segun_instruccion(funcion, instruccion);

			resultado = funcion(&tcb);

			loggear_trace("Resultado de la ejecucion: %d", resultado);

			quantum--;
		}

		if (resultado == OK)
		{
			loggear_info("Se termino el quantum");
			resultado = FIN_QUANTUM;
		}

		if (informar_a_kernel_de_finalizacion(tcb, resultado)
			== FALLO_INFORME_A_KERNEL)
		{
			loggear_error("No pudo informar al kernel");
			_liberar_recursos();
			error_show(" Al enviar informe a kernel");
			return 0;
		}
	}

	_liberar_recursos();

	return 0;
}

void _liberar_recursos()
{
	loggear_info("Liberando recursos para cierre...");
	liberar_configuraciones();
	liberar_dic_de_instrucciones();
	desconectarse();
	finalizar_loggeo();
}
