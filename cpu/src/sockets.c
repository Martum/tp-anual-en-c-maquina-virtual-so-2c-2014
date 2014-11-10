/*
 * memoria.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include "sockets.h"

#define PID_KERNEL 1;

sock_t* MEMORIA;
sock_t* kernel;

resultado_t _enviar_y_recibir(sock_t* socket, char* chorro_a_enviar,
	uint32_t len_a_enviar, char* respuesta)
{
	loggear_trace("Me preparo para enviar %d bytes", len_a_enviar);

	if (enviar(socket, chorro_a_enviar, &len_a_enviar) == -1)
	{
		loggear_warning("No pudo mandar el paquete de %d bytes", len_a_enviar);
		return FALLO_COMUNICACION;
	}

	loggear_trace("Envio con exito paquete de %d bytes", len_a_enviar);

	char* mensaje_recibido;
	uint32_t len_devolucion;

	loggear_trace("Me preparo para recibir respuesta");

	if (recibir(socket, &mensaje_recibido, &len_devolucion) == -1)
	{
		loggear_warning("No pudo recibir respuesta");
		return FALLO_COMUNICACION;
	}

	loggear_trace("Recibio con exito paquete de %d bytes", len_devolucion);

	memcpy(respuesta, mensaje_recibido, len_devolucion);

	return OK;
}

resultado_t _conectar(sock_t** socket, char* ip, uint32_t puerto)
{
	loggear_trace("IP %s -- Puerto %d", ip, puerto);

	*socket = crear_socket_hablador(ip, puerto);

	if (conectar(*socket) == -1)
	{
		loggear_warning("No pudo conectarse a ip: %s y puerto: %d", ip, puerto);
		return FALLO_CONEXION;
	}

	loggear_trace("Conexion realizada con exito");

	return OK;
}

resultado_t _mandar_soy_cpu_a_kernel()
{
	pedido_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = SOY_CPU;

	char* chorro_de_envio = serializar_pedido_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_pedido_t_serializado());

	loggear_trace("Me preparo para enviar y recibir SOY_CPU");

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		loggear_warning("Problema al comunicarse con kernel");
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

	pedido_t* respuesta = deserializar_pedido_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->flag != BIENVENIDO)
	{
		loggear_warning("El mensaje de respuesta no es el esperado, sino %d",
			respuesta->flag);
		free(respuesta);
		return FALLO_COMUNICACION;
	}

	loggear_trace("Respuesta recibida correcta");

	loggear_trace("Comunicacion con kernel realizada con exito");

	free(respuesta);

	return OK;
}

resultado_t conectar_con_kernel()
{
	loggear_trace("Intento conectarme con kernel");

	if (_conectar(&kernel, ip_kernel(), puerto_kernel()) == FALLO_CONEXION)
		return FALLO_CONEXION;

	if (_mandar_soy_cpu_a_kernel() == FALLO_COMUNICACION)
		return FALLO_CONEXION;

	loggear_info("Conexion con kernel realizada con exito");

	return OK;
}

resultado_t conectar_con_memoria()
{
	loggear_trace("Intento conectarme con memoria");

	if (_conectar(&MEMORIA, ip_msp(), puerto_msp()) == FALLO_CONEXION)
		return FALLO_CONEXION;

	loggear_info("Conexion con memoria realizada con exito");

	return OK;
}

resultado_t _mandar_desconexion_cpu_a_memoria()
{
	char* chorro_a_enviar = malloc(sizeof(resultado_t));
	uint32_t tamanio = sizeof(resultado_t);
	resultado_t resultado = DESCONEXION_CPU;

	memcpy(chorro_a_enviar, &resultado, tamanio);

	loggear_trace("Me preparo para enviar DESCONEXION_CPU a memoria");

	enviar(MEMORIA, chorro_a_enviar, &tamanio);

	loggear_trace("Envio de mensaje de DESCONEXION_CPU realizado con exito");

	free(chorro_a_enviar);

	return OK;
}

resultado_t _mandar_desconexion_cpu_a_kernel()
{
	char* chorro_a_enviar = malloc(sizeof(resultado_t));
	uint32_t tamanio = sizeof(resultado_t);
	resultado_t resultado = DESCONEXION_CPU;

	memcpy(chorro_a_enviar, &resultado, tamanio);

	loggear_trace("Me preparo para enviar DESCONEXION_CPU a kernel");

	enviar(kernel, chorro_a_enviar, &tamanio);

	loggear_trace("Envio de mensaje de DESCONEXION_CPU realizado con exito");

	free(chorro_a_enviar);

	return OK;
}

resultado_t desconectar_memoria()
{
	loggear_trace("Intento desconectarme de memoria");
	_mandar_desconexion_cpu_a_memoria();
	cerrar_liberar(MEMORIA);
	loggear_info("Desconexion de memoria realizada con exito");

	return OK;
}

resultado_t desconectar_kernel()
{
	loggear_trace("Intento desconectarme de kernel");
	_mandar_desconexion_cpu_a_kernel();
	cerrar_liberar(kernel);
	loggear_info("Desconexion de kernel realizada con exito");

	return OK;
}

resultado_t desconectarse()
{
	desconectar_memoria();
	// TODO descomentar (solo comentado para pruebas de memoria)
//	desconectar_kernel();

	return OK;
}

resultado_t crear_segmento(direccion pid, uint32_t tamanio,
	direccion* direccion)
{
	loggear_trace("Preparando mensaje para crear segmento");

	pedido_de_crear_segmento_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = CREA_UN_SEGMENTO;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.tamano = tamanio;

	char* chorro_de_envio = serializar_pedido_de_crear_segmento_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(
		tamanio_respuesta_de_crear_segmento_t_serializado());

	loggear_trace("PID %d", pid);
	loggear_trace("Tamanio del segmento %d", tamanio);

	if (_enviar_y_recibir(MEMORIA, chorro_de_envio,
		tamanio_pedido_de_crear_segmento_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_CREACION_DE_SEGMENTO;
	}

	loggear_trace("Recibi respuesta por creacion de segmento");

	respuesta_de_crear_segmento_t* respuesta =
		deserializar_respuesta_de_crear_segmento_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->resultado == ERROR_DE_MEMORIA_LLENA)
	{
		loggear_warning(
			"La creacion no pudo realizarse porque la memoria estaba llena");
		free(respuesta);
		return FALLO_CREACION_DE_SEGMENTO;
	}

	*direccion = respuesta->direccion_virtual;

	free(respuesta);

	loggear_debug("Creacion de segmento satisfactoria");

	loggear_trace("Direccion del segmento %d", *direccion);

	return OK;
}

resultado_t destruir_segmento(direccion pid, direccion direccion)
{
	loggear_trace("Preparando mensaje para destruir segmento");

	pedido_de_destruir_segmento_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = DESTRUI_SEGMENTO;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;

	char* chorro_de_envio = serializar_pedido_de_destruir_segmento_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_respuesta_t_serializado());

	loggear_trace("PID: %d", pid);
	loggear_trace("Direccion %d", direccion);

	if (_enviar_y_recibir(MEMORIA, chorro_de_envio,
		tamanio_pedido_de_destruir_segmento_t_serializado(),
		chorro_de_respuesta) == FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_DESTRUCCION_DE_SEGMENTO;
	}

	loggear_trace("Recibi respuesta por destruccion de segmento");

	respuesta_t* respuesta = deserializar_respuesta_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->resultado == ERROR_NO_ENCUENTRO_SEGMENTO)
	{
		loggear_warning(
			"La destruccion no pudo realizarse por no encontrar el segmetno");
		free(respuesta);
		return FALLO_DESTRUCCION_DE_SEGMENTO;
	}

	free(respuesta);

	loggear_debug("Destruccion de segmento satisfactoria");

	return OK;
}

resultado_t leer_de_memoria(direccion pid, direccion direccion,
	uint32_t cantidad_de_bytes, char* buffer)
{
	loggear_trace("Preparando mensaje para leer de memoria");

	pedido_de_leer_de_memoria_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = LEE_DE_MEMORIA;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;
	cuerpo_del_mensaje.tamano = cantidad_de_bytes;

	char* chorro_de_envio = serializar_pedido_de_leer_de_memoria_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(
		tamanio_respuesta_de_leer_de_memoria_t_serializado(cantidad_de_bytes));

	loggear_trace("Cantidad de bytes a leer %d", cantidad_de_bytes);

	if (_enviar_y_recibir(MEMORIA, chorro_de_envio,
		tamanio_pedido_de_leer_de_memoria_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_LECTURA_DE_MEMORIA;
	}

	loggear_trace("Recibi respuesta por lectura de memoria");

	respuesta_de_leer_de_memoria_t* respuesta =
		deserializar_respuesta_de_leer_de_memoria_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->resultado == SEGMENTATION_FAULT)
	{
		loggear_warning(
			"La lectura no pudo realizarse por violacion de segmento");
//		free(respuesta->bytes_leido);
		free(respuesta);
		return FALLO_LECTURA_DE_MEMORIA;
	}

	memcpy(buffer, respuesta->bytes_leido, 1);

//	free(respuesta->bytes_leido);

	loggear_debug("Lectura de memorira satisfactoria");

	loggear_trace("Cantidad de bytes leidos %d", respuesta->tamano);

//	buffer[1] = '\0';

	loggear_trace("Bytes: %s", buffer);

	free(respuesta);

	return OK;
}

resultado_t escribir_en_memoria(direccion pid, direccion direccion,
	uint32_t cantidad_de_bytes, char* bytes_a_escribir)
{
	loggear_trace("Preparando mensaje escribir en memoria");

	pedido_de_escribir_en_memoria_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = ESCRIBI_EN_MEMORIA;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;
	cuerpo_del_mensaje.bytes_a_escribir = bytes_a_escribir;
	cuerpo_del_mensaje.tamano = cantidad_de_bytes;

	char* chorro_de_envio = serializar_pedido_de_escribir_en_memoria_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_respuesta_t_serializado());

	loggear_trace("Cantidad de bytes a escribir %d", cantidad_de_bytes);
	loggear_trace("Bytes %s", bytes_a_escribir);

	if (_enviar_y_recibir(MEMORIA, chorro_de_envio,
		tamanio_pedido_de_escribir_en_memoria_t_serializado(cantidad_de_bytes),
		chorro_de_respuesta) == FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_ESCRITURA_EN_MEMORIA;
	}

	loggear_trace("Recibi respuesta por escritura en memoria");

	respuesta_t* respuesta = deserializar_respuesta_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->resultado == SEGMENTATION_FAULT)
	{
		loggear_warning(
			"La escritura no pudo realizarse por violacion de segmento");
		free(respuesta);
		return FALLO_ESCRITURA_EN_MEMORIA;
	}

	free(respuesta);

	loggear_debug("Escritura en memoria satisfactoria");

	return OK;
}

resultado_t pedir_tcb(tcb_t* tcb, int32_t* quantum)
{
	loggear_trace("Preparando mensaje para pedir TCB");

	pedido_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = MANDA_TCB;

	char* chorro_de_envio = serializar_pedido_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(
		tamanio_respuesta_de_nuevo_tcb_t_serializado());

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_PEDIDO_DE_TCB;
	}

	loggear_trace("Recibi respuesta por pedido de TCB");

	respuesta_de_nuevo_tcb_t* respuesta = deserializar_respuesta_de_nuevo_tcb_t(
		chorro_de_respuesta);

	*tcb = *respuesta->tcb;
	*quantum = respuesta->quantum;

	free(chorro_de_envio);
	free(chorro_de_respuesta);
	free(respuesta->tcb);
	free(respuesta);

	loggear_debug("Pedido de TCB satisfactorio");

	loggear_trace("PID TCB %d -- TID TCB %d", tcb->pid, tcb->tid);
	loggear_trace("Quantum %d", *quantum);

	return OK;
}

resultado_t informar_a_kernel_de_finalizacion(tcb_t tcb, resultado_t res)
{
	if (res == EXCEPCION_POR_INTERRUPCION)
	{
		loggear_trace("Preparando mensaje para informar a kernel interrupcion");

		loggear_trace("Busco direccion de interrupcion");

		int32_t direccion;
		leer_numero(&tcb, &direccion);

		loggear_debug("Direccion de interrupcion %d", direccion);

		pedido_interrupcion_t cuerpo_del_mensaje;
		cuerpo_del_mensaje.flag = INTERRUPCION;
		cuerpo_del_mensaje.tcb = &tcb;
		cuerpo_del_mensaje.direccion_de_memoria = direccion;

		char* chorro_de_envio = serializar_pedido_interrupcion_t(
			&cuerpo_del_mensaje);
		char* chorro_de_respuesta = malloc(sizeof(resultado_t));

		if (_enviar_y_recibir(kernel, chorro_de_envio,
			tamanio_pedido_interrupcion_t_serializado(), chorro_de_respuesta)
			== FALLO_COMUNICACION)
		{
			free(chorro_de_envio);
			free(chorro_de_respuesta);
			return FALLO_INFORME_A_KERNEL;
		}

		loggear_trace("Recibi respuesta por informe de interrupcion");

		resultado_t resultado = *chorro_de_respuesta;

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		if (resultado != COMPLETADO_OK)
		{
			loggear_warning("No se pudo completar el informe de interrupcion");
			return FALLO_INFORME_A_KERNEL;
		}

		loggear_debug("Informe de interrupcion satisfactorio");

		return OK;
	}

	loggear_trace("Preparando mensaje para informar al kernel un resultado");

	pedido_con_resultado_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.tcb = &tcb;
	cuerpo_del_mensaje.resultado = res;

	char* chorro_de_envio = serializar_pedido_con_resultado_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));

	loggear_trace("PID TCB %d -- TID TCB %d", tcb.pid, tcb.tid);
	loggear_trace("Resultado %d", res);

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_con_resultado_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_INFORME_A_KERNEL;
	}

	loggear_trace("Recibi respuesta por informe de resultado");

	resultado_t resultado = *chorro_de_respuesta;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (resultado != COMPLETADO_OK)
	{
		loggear_warning("No se pudo completar el informe de resultado");
		return FALLO_INFORME_A_KERNEL;
	}

	loggear_debug("Informe de resultado satisfactorio");

	return OK;
}

resultado_t _obtener(tcb_t* tcb, char* buffer, uint32_t bytes_a_leer)
{
	loggear_trace("Me preparo para obtener %d bytes", bytes_a_leer);

	uint32_t pid_a_leer = PID_KERNEL
	;

	if (!tcb->km)
		pid_a_leer = tcb->pid;

	loggear_trace("PID TCB %d -- Direccion a leer %d", pid_a_leer, tcb->pc);
	loggear_trace("Modo kernel %d", tcb->km);

	if (leer_de_memoria(pid_a_leer, tcb->pc, bytes_a_leer, buffer)
		== FALLO_LECTURA_DE_MEMORIA)
		return FALLO_LECTURA_DE_MEMORIA;

	loggear_debug("Obtencion de bytes satisfactoria");

	actualizar_pc(tcb, tcb->pc + bytes_a_leer);

	loggear_trace("PC nuevo %d", tcb->pc);

	return OK;
}

resultado_t leer_proxima_instruccion(tcb_t* tcb, instruccion_t instruccion)
{
	loggear_trace("Me preparo para leer una instruccion");

	if (_obtener(tcb, instruccion, sizeof(instruccion_t) - 1)
		== FALLO_LECTURA_DE_MEMORIA)
		return FALLO_LECTURA_DE_MEMORIA;

	instruccion[4] = '\0';

	loggear_debug("Lectura de instruccion satisfactoria");

	loggear_trace("Instruccion %s", instruccion);

	return OK;
}

resultado_t leer_registro(tcb_t* tcb, char* registro)
{
	loggear_trace("Me preparo para leer registro");

	if (_obtener(tcb, registro, sizeof(char)) == FALLO_LECTURA_DE_MEMORIA)
		return FALLO_LECTURA_DE_MEMORIA;

	loggear_debug("Lectura de registro satisfactoria");

	loggear_trace("Registro %c", registro);

	return OK;
}

resultado_t leer_numero(tcb_t* tcb, int32_t* numero)
{
	loggear_trace("Me preparo para leer numero");

	char buffer[3];

	if (_obtener(tcb, buffer, sizeof(int32_t)) == FALLO_LECTURA_DE_MEMORIA)
		return FALLO_LECTURA_DE_MEMORIA;

	unir_bytes(numero, buffer);

	loggear_debug("Lectura de numero satisfactoria");

	loggear_trace("Numero %d", *numero);

	return OK;
}

resultado_t comunicar_entrada_estandar(tcb_t* tcb, uint32_t bytes_a_leer,
	uint32_t* bytes_leidos, char* buffer, idetificador_tipo_t identificador)
{
	loggear_trace("Preparando mensaje comunicar entrada estandar");

	pedido_entrada_estandar_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.pid = tcb->pid;
	cuerpo_del_mensaje.identificador_de_tipo = identificador;

	uint32_t len_a_enviar = tamanio_pedido_entrada_estandar_t_serializado();

	char* chorro_de_envio = serializar_pedido_entrada_estandar_t(
		&cuerpo_del_mensaje);

	loggear_trace("PID %s", tcb->pid);
	loggear_trace("Cantidad de bytes de entrada %d", bytes_a_leer);
	loggear_trace("Tipo %d", identificador);

	if (enviar(kernel, chorro_de_envio, &len_a_enviar) == -1)
		return FALLO_COMUNICACION;

	char* chorro_de_respuesta;
	uint32_t len_devolucion;

	if (recibir(kernel, &chorro_de_respuesta, &len_devolucion) == -1)
		return FALLO_COMUNICACION;

	loggear_trace("Recibi respuesta por entrada estandar");

	respuesta_entrada_estandar_t* respuesta =
		deserializar_respuesta_entrada_estandar_t(chorro_de_respuesta);

	*bytes_leidos = respuesta->tamanio;
	buffer = respuesta->cadena;

	free(chorro_de_envio);
	free(chorro_de_respuesta);
	free(respuesta->cadena);
	free(respuesta);

	loggear_debug("Entrada estandar satisfactoria");

	loggear_trace("Entrada %s", buffer);
	loggear_trace("Tamanio %d", *bytes_leidos);

	return OK;
}

// TODO avisar a mati que tiene que serializar de nuevo pedido salida estandar
resultado_t comunicar_salida_estandar(tcb_t* tcb, uint32_t bytes_a_enviar,
	char* buffer, idetificador_tipo_t identificador)
{
	loggear_trace("Me preparo para comunicar salida estandar");

	pedido_salida_estandar_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.pid = tcb->pid;
	cuerpo_del_mensaje.tamanio = bytes_a_enviar;
	cuerpo_del_mensaje.cadena_de_texto = buffer;
	cuerpo_del_mensaje.identificador_de_tipo = identificador;

	char* chorro_de_envio = serializar_pedido_salida_estandar_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));

	loggear_trace("PID %d", tcb->pid);
	loggear_trace("Cadena a imprimir %s", bytes_a_enviar);
	loggear_trace("Tipo %d", identificador);

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_salida_estandar_t_serializado(bytes_a_enviar),
		chorro_de_respuesta) == FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

	loggear_trace("Recibi respuesta por salida estandar");

	resultado_t resultado = *chorro_de_respuesta;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (resultado != COMPLETADO_OK)
	{
		loggear_warning("La salida estandar no se realizo correctamente");
		return ERROR_EN_EJECUCION;
	}

	loggear_debug("Salida estantar satisfactoria");

	return OK;
}

resultado_t comunicar_nuevo_tcb(tcb_t* nuevo_tcb)
{
	loggear_trace("Me preparo para comunicar nuevo tcb");

	pedido_crear_hilo_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.tcb = nuevo_tcb;

	char* chorro_de_envio = serializar_pedido_crear_hilo_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));

	loggear_trace("PID nuevo tcb %d", nuevo_tcb);

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_crear_hilo_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

	loggear_trace("Recibi respuesta por nuevo tcb");

	resultado_t resultado = *chorro_de_respuesta;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (resultado != COMPLETADO_OK)
	{
		loggear_warning("Nuevo tcb no se realizo correctamente");
		return ERROR_EN_EJECUCION;
	}

	loggear_debug("Comunicacion nuevo tcb satisfactorio");

	return OK;
}

resultado_t comunicar_join(uint32_t tid_llamador, uint32_t tid_esperador)
{
	loggear_trace("Me preparo para comunicar join");

	pedido_join_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.tid_llamador = tid_llamador;
	cuerpo_del_mensaje.tid_esperador = tid_esperador;

	char* chorro_de_envio = serializar_pedido_join_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));

	loggear_trace("TID llamador %d", tid_llamador);
	loggear_trace("TID esperador %d", tid_esperador);

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_join_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

	loggear_trace("Recibi respuesta por join");

	resultado_t resultado = *chorro_de_respuesta;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (resultado != COMPLETADO_OK)
	{
		loggear_warning("Join no se realizo correctamente");
		return ERROR_EN_EJECUCION;
	}

	loggear_debug("Comunicacion join satisfactorio");

	return OK;
}

resultado_t comunicar_bloquear(tcb_t* tcb, uint32_t id_recurso)
{
	loggear_trace("Me preparo para comunicar bloquear");

	pedido_bloquear_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.tcb = tcb;
	cuerpo_del_mensaje.identificador_de_recurso = id_recurso;

	char* chorro_de_envio = serializar_pedido_bloquear_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));

	loggear_trace("PID %d", tcb->pid);
	loggear_trace("Id del recurso %d", id_recurso);

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_bloquear_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

	loggear_trace("Recibi respuesta por bloqueo");

	resultado_t resultado = *chorro_de_respuesta;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (resultado != COMPLETADO_OK)
	{
		loggear_warning("Bloqueo no se realizo correctamente");
		return ERROR_EN_EJECUCION;
	}

	loggear_debug("Comunicar bloqueo satisfactorio");

	return OK;
}

resultado_t comunicar_despertar(tcb_t* tcb, uint32_t id_recurso)
{
	loggear_trace("Me preparo para comunicar despertar");

	pedido_despertar_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.identificador_de_recurso = id_recurso;

	char* chorro_de_envio = serializar_pedido_despertar_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));

	loggear_trace("PID %d", tcb->pid);
	loggear_trace("Id del recurso %d", id_recurso);

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_despertar_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

	loggear_trace("Recibi respuesta por despertar");

	resultado_t resultado = *chorro_de_respuesta;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (resultado != COMPLETADO_OK)
	{
		loggear_warning("Comunicar despertar no se realizo correctamente");
		return ERROR_EN_EJECUCION;
	}

	loggear_debug("Comunicar despertar satisfactorio");

	return OK;
}

// TODO eliminar (ya no sirve mas)
// TODO avisar a mati que tiene que implementar las serializaciones
//resultado_t pedir_tid_a_kernel(tcb_t tcb, direccion* nuevo_tid)
//{
//	pedido_nuevo_tid_t cuerpo_del_mensaje;
//	cuerpo_del_mensaje.flag = DAME_TID;
//	cuerpo_del_mensaje.tcb = &tcb;
//
//	char* chorro_de_envio = serializar_pedido_nuevo_tid_t(&cuerpo_del_mensaje);
//	char* chorro_de_respuesta = malloc(
//		tamanio_respuesta_nuevo_tid_t_serializado());
//
//	if (_enviar_y_recibir(kernel, chorro_de_envio,
//		tamanio_pedido_nuevo_tid_t_serializado(), chorro_de_respuesta)
//		== FALLO_COMUNICACION)
//	{
//		free(chorro_de_envio);
//		free(chorro_de_respuesta);
//		return FALLO_COMUNICACION;
//	}
//
//	respuesta_nuevo_tid_t* respuesta = deserializar_respuesta_nuevo_tid_t(
//		chorro_de_respuesta);
//
//	*nuevo_tid = respuesta->tid;
//
//	free(chorro_de_envio);
//	free(chorro_de_respuesta);
//
//	return OK;
//}

// TODO eliminar (ya no se hace falta)
//void pedir_al_kernel_tamanio_stack(uint32_t *tamano_stack)
//{
//
//}
