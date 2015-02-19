/*
 * memoria.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#include "sockets.h"

#define PID_KERNEL 1;

sock_t* memoria = NULL;
sock_t* kernel = NULL;

resultado_t _enviar_y_recibir(sock_t* socket, char* chorro_a_enviar,
	uint32_t len_a_enviar, char* respuesta)
{
	if (enviar(socket, chorro_a_enviar, &len_a_enviar) == -1)
	{
		loggear_warning("No pudo mandar el paquete de %d bytes", len_a_enviar);
		return FALLO_COMUNICACION;
	}

	char* mensaje_recibido;
	uint32_t len_devolucion;

	if (recibir(socket, &mensaje_recibido, &len_devolucion) == -1)
	{
		loggear_warning("No pudo recibir respuesta");
		return FALLO_COMUNICACION;
	}

	memcpy(respuesta, mensaje_recibido, len_devolucion);

	return OK;
}

resultado_t _conectar(sock_t** socket, char* ip, uint32_t puerto)
{
	*socket = crear_socket_hablador(ip, puerto);

	if (conectar(*socket) == -1)
	{
		*socket = NULL;
		return FALLO_CONEXION;
	}

	return OK;
}

resultado_t _mandar_soy_cpu_a_kernel()
{
	pedido_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = SOY_CPU;

	char* chorro_de_envio = serializar_pedido_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_pedido_t_serializado());

	loggear_trace("Envio SOY_CPU a kernel");

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

	free(respuesta);

	return OK;
}

resultado_t conectar_con_kernel()
{
	loggear_trace("Intento conectarme con kernel [IP: %s, Puerto %d]",
		ip_kernel(), puerto_kernel());

	if (_conectar(&kernel, ip_kernel(), puerto_kernel()) == FALLO_CONEXION)
		return FALLO_CONEXION;

	if (_mandar_soy_cpu_a_kernel() == FALLO_COMUNICACION)
		return FALLO_CONEXION;

	loggear_info("Conexion con kernel realizada con exito\n");

	return OK;
}

resultado_t conectar_con_memoria()
{
	loggear_trace("Intento conectarme con memoria [IP: %s, Puerto %d]",
		ip_msp(), puerto_msp());

	if (_conectar(&memoria, ip_msp(), puerto_msp()) == FALLO_CONEXION)
		return FALLO_CONEXION;

	loggear_info("Conexion con memoria realizada con exito\n");

	return OK;
}

void _mandar_desconexion_cpu(sock_t* socket)
{
	flag_t resultado = DESCONEXION_CPU;
	uint32_t tamanio = sizeof(flag_t);
	char* chorro_a_enviar = malloc(tamanio);

	memcpy(chorro_a_enviar, &resultado, tamanio);

	enviar(socket, chorro_a_enviar, &tamanio);

	free(chorro_a_enviar);
}

void _mandar_desconexion_cpu_a_memoria()
{
	loggear_trace("Envio DESCONEXION_CPU a memoria");

	_mandar_desconexion_cpu(memoria);

	loggear_trace("Envio de mensaje de DESCONEXION_CPU realizado con exito");
}

void _mandar_desconexion_cpu_a_kernel()
{
	loggear_trace("Envio DESCONEXION_CPU a kernel");

	_mandar_desconexion_cpu(kernel);

	loggear_trace("Envio de mensaje de DESCONEXION_CPU realizado con exito");
}

void desconectar_memoria()
{
	loggear_trace("Intento desconectarme de memoria");

	_mandar_desconexion_cpu_a_memoria();

	cerrar_liberar(memoria);

	loggear_info("Desconexion de memoria realizada con exito");
}

void desconectar_kernel()
{
	loggear_trace("Intento desconectarme de kernel");

	_mandar_desconexion_cpu_a_kernel();

	cerrar_liberar(kernel);

	loggear_info("Desconexion de kernel realizada con exito");
}

void desconectarse()
{
	if (memoria != NULL )
		desconectar_memoria();

	if (kernel != NULL )
		desconectar_kernel();
}

resultado_t crear_segmento(direccion pid, uint32_t tamanio,
	direccion* direccion)
{
	loggear_trace("Creo segmento [PID: %d, Tamaño: %d]", pid, tamanio);

	pedido_de_crear_segmento_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = CREA_UN_SEGMENTO;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.tamano = tamanio;

	char* chorro_de_envio = serializar_pedido_de_crear_segmento_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(
		tamanio_respuesta_de_crear_segmento_t_serializado());

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_de_crear_segmento_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_CREACION_DE_SEGMENTO;
	}

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

	loggear_debug("Creacion de segmento satisfactoria: %d", *direccion);

	return OK;
}

resultado_t destruir_segmento(direccion pid, direccion direccion)
{
	loggear_trace("Destruyo segmento [PID: %d, Direccion: %d]", pid, direccion);

	pedido_de_destruir_segmento_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = DESTRUI_SEGMENTO;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;

	char* chorro_de_envio = serializar_pedido_de_destruir_segmento_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_respuesta_t_serializado());

	if (_enviar_y_recibir(memoria, chorro_de_envio,
		tamanio_pedido_de_destruir_segmento_t_serializado(),
		chorro_de_respuesta) == FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_DESTRUCCION_DE_SEGMENTO;
	}

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
	loggear_trace(
		"Leo memoria [PID: %d, Direccion a leer: %d - %d, Direccion a leer: %x - %x]",
		pid, direccion, direccion + cantidad_de_bytes - 1, direccion,
		direccion + cantidad_de_bytes - 1);

	pedido_de_leer_de_memoria_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = LEE_DE_MEMORIA;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;
	cuerpo_del_mensaje.tamano = cantidad_de_bytes;

	char* chorro_de_envio = serializar_pedido_de_leer_de_memoria_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(
		tamanio_respuesta_de_leer_de_memoria_t_serializado(cantidad_de_bytes));

	if (_enviar_y_recibir(memoria, chorro_de_envio,
		tamanio_pedido_de_leer_de_memoria_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_LECTURA_DE_MEMORIA;
	}

	respuesta_de_leer_de_memoria_t* respuesta =
		deserializar_respuesta_de_leer_de_memoria_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->resultado == SEGMENTATION_FAULT)
	{
		loggear_warning(
			"La lectura no pudo realizarse por violacion de segmento");
		free(respuesta->bytes_leido);
		free(respuesta);
		return FALLO_LECTURA_DE_MEMORIA;
	}

	memcpy(buffer, respuesta->bytes_leido, respuesta->tamano);

	free(respuesta->bytes_leido);
	free(respuesta);

	loggear_trace("Lectura de memorira satisfactoria");

	return OK;
}

resultado_t escribir_en_memoria(direccion pid, direccion direccion,
	uint32_t cantidad_de_bytes, char* bytes_a_escribir)
{
	loggear_debug(
		"Escribo en memoria [Direcciones a escribir: %d - %d, Direcciones a escribir: %x - %x]",
		direccion, direccion + cantidad_de_bytes - 1, direccion,
		direccion + cantidad_de_bytes - 1);

	pedido_de_escribir_en_memoria_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = ESCRIBI_EN_MEMORIA;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;
	cuerpo_del_mensaje.bytes_a_escribir = bytes_a_escribir;
	cuerpo_del_mensaje.tamano = cantidad_de_bytes;

	char* chorro_de_envio = serializar_pedido_de_escribir_en_memoria_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_respuesta_t_serializado());

	if (_enviar_y_recibir(memoria, chorro_de_envio,
		tamanio_pedido_de_escribir_en_memoria_t_serializado(cantidad_de_bytes),
		chorro_de_respuesta) == FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_ESCRITURA_EN_MEMORIA;
	}

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
	loggear_trace("Pido TCB");

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

	respuesta_de_nuevo_tcb_t* respuesta = deserializar_respuesta_de_nuevo_tcb_t(
		chorro_de_respuesta);

	*tcb = *respuesta->tcb;
	*quantum = respuesta->quantum;

	free(chorro_de_envio);
	free(chorro_de_respuesta);
	free(respuesta->tcb);
	free(respuesta);

	loggear_debug("Pedido de TCB satisfactorio [PID: %d, TID %d, Quantum %d]\n",
		tcb->pid, tcb->tid, *quantum);

	return OK;
}

resultado_t informar_a_kernel_de_finalizacion(tcb_t tcb, resultado_t res)
{
	if (res == EXCEPCION_POR_INTERRUPCION)
	{
		loggear_trace("Busco direccion de interrupcion");

		int32_t direccion;
		leer_numero(&tcb, &direccion);

		ansisop_ejecucion_instruccion3("INTE", direccion);

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

		resultado_t resultado = *chorro_de_respuesta;

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		if (resultado != COMPLETADO_OK)
		{
			loggear_warning(
				"No se pudo completar el informe de interrupcion\n");
			return FALLO_INFORME_A_KERNEL;
		}

		loggear_debug("Informe de interrupcion satisfactorio\n");

		return OK;
	}

	loggear_trace("Informo al kernel un resultado [PID: %d, Resultado: %d]",
		tcb.pid, res);

	pedido_con_resultado_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.tcb = &tcb;
	cuerpo_del_mensaje.resultado = res;

	char* chorro_de_envio = serializar_pedido_con_resultado_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));


	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_con_resultado_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_INFORME_A_KERNEL;
	}

	resultado_t resultado = *chorro_de_respuesta;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (resultado != COMPLETADO_OK)
	{
		loggear_warning("No se pudo completar el informe de resultado");
		return FALLO_INFORME_A_KERNEL;
	}

	loggear_debug("Informe de resultado satisfactorio\n");

	return OK;
}

resultado_t _obtener(tcb_t* tcb, char* buffer, uint32_t bytes_a_leer)
{
	uint32_t pid_a_leer = PID_KERNEL
	;

	if (!tcb->km)
		pid_a_leer = tcb->pid;

	if (leer_de_memoria(pid_a_leer, tcb->pc, bytes_a_leer, buffer)
		== FALLO_LECTURA_DE_MEMORIA)
		return FALLO_LECTURA_DE_MEMORIA;

	actualizar_pc(tcb, tcb->pc + bytes_a_leer);

	return OK;
}

resultado_t leer_proxima_instruccion(tcb_t* tcb, instruccion_t instruccion)
{
	loggear_trace("Leo instruccion");

	if (_obtener(tcb, instruccion, 4) == FALLO_LECTURA_DE_MEMORIA)
		return FALLO_LECTURA_DE_MEMORIA;

	instruccion[4] = '\0';

	loggear_debug("Lectura de instruccion satisfactoria: %s", instruccion);

	return OK;
}

resultado_t leer_registro(tcb_t* tcb, char* registro)
{
	loggear_trace("Leo registro");

	if (_obtener(tcb, registro, sizeof(char)) == FALLO_LECTURA_DE_MEMORIA)
		return FALLO_LECTURA_DE_MEMORIA;

	loggear_debug("Lectura de registro satisfactoria: %c", *registro);

	return OK;
}

resultado_t leer_numero(tcb_t* tcb, int32_t* numero)
{
	loggear_trace("Leo numero");

	char* buffer = malloc(sizeof(int32_t));

	if (_obtener(tcb, buffer, sizeof(int32_t)) == FALLO_LECTURA_DE_MEMORIA)
	{
		free(buffer);
		return FALLO_LECTURA_DE_MEMORIA;
	}

	memcpy(numero, buffer, sizeof(int32_t));

	free(buffer);

	loggear_debug("Lectura de numero satisfactoria: %d", *numero);

	return OK;
}

resultado_t comunicar_entrada_estandar(tcb_t* tcb, uint32_t bytes_a_leer,
	uint32_t* bytes_leidos, char* buffer, idetificador_tipo_t identificador)
{
	loggear_debug(
		"Comunico entrada estandar [PID: %d, Cantidad de bytes a leer %d, Tipo: %d]",
		tcb->pid, bytes_a_leer, identificador);

	pedido_entrada_estandar_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = ENTRADA_ESTANDAR;
	cuerpo_del_mensaje.pid = tcb->pid;
	cuerpo_del_mensaje.tid = tcb->tid;
	cuerpo_del_mensaje.identificador_de_tipo = identificador;

	uint32_t len_a_enviar = tamanio_pedido_entrada_estandar_t_serializado();

	char* chorro_de_envio = serializar_pedido_entrada_estandar_t(
		&cuerpo_del_mensaje);

	if (enviar(kernel, chorro_de_envio, &len_a_enviar) == -1)
		return FALLO_COMUNICACION;

	char* chorro_de_respuesta;
	uint32_t len_devolucion;

	if (recibir(kernel, &chorro_de_respuesta, &len_devolucion) == -1)
		return FALLO_COMUNICACION;

	respuesta_entrada_estandar_t* respuesta =
		deserializar_respuesta_entrada_estandar_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->resultado == FALLO_ENTRADA_ESTANDAR)
	{
		free(respuesta->cadena);
		free(respuesta);
		loggear_warning("La entrada estandar no se realizo correctamente");
		return ERROR_EN_EJECUCION;
	}

	*bytes_leidos = respuesta->tamanio;
	memcpy(buffer, respuesta->cadena, *bytes_leidos);

	free(respuesta);
	free(respuesta->cadena);

	loggear_debug("Entrada estandar satisfactoria [Entrada %s, Tamaño %d]",
		buffer, *bytes_leidos);

	return OK;
}

resultado_t comunicar_salida_estandar(tcb_t* tcb, uint32_t bytes_a_enviar,
	char* buffer, idetificador_tipo_t identificador)
{
	loggear_debug(
		"Comunico salida estandar [PID: %d, Cadena a imprimir %s, Tipo %d]",
		tcb->pid, buffer, identificador);

	pedido_salida_estandar_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = SALIDA_ESTANDAR;
	cuerpo_del_mensaje.pid = tcb->pid;
	cuerpo_del_mensaje.tamanio = bytes_a_enviar;
	cuerpo_del_mensaje.cadena_de_texto = buffer;
	cuerpo_del_mensaje.identificador_de_tipo = identificador;

	char* chorro_de_envio = serializar_pedido_salida_estandar_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_salida_estandar_t_serializado(bytes_a_enviar),
		chorro_de_respuesta) == FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

	resultado_t resultado = *chorro_de_respuesta;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (resultado != COMPLETADO_OK)
	{
		loggear_warning("La salida estandar no se realizo correctamente");
		return ERROR_EN_EJECUCION;
	}

	loggear_debug("Salida estandar satisfactoria");

	return OK;
}

resultado_t comunicar_nuevo_tcb(tcb_t* nuevo_tcb, uint32_t* nuevo_tid)
{
	loggear_debug("Comunico nuevo tcb [PID %d]", nuevo_tcb->pid);

	pedido_crear_hilo_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = CREAR_HILO;
	cuerpo_del_mensaje.tcb = nuevo_tcb;

	char* chorro_de_envio = serializar_pedido_crear_hilo_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(respuesta_crear_hilo_t));

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_crear_hilo_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

	respuesta_crear_hilo_t* respuesta = deserializar_respuesta_crear_hilo_t(
		chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta->resultado != COMPLETADO_OK)
	{
		free(respuesta);
		loggear_warning("Nuevo tcb no se realizo correctamente");
		return ERROR_EN_EJECUCION;
	}

	*nuevo_tid = respuesta->nuevo_tid;

	free(respuesta);

	loggear_debug("Comunicacion nuevo tcb satisfactorio [Nuevo tid: %d]",
		*nuevo_tid);

	return OK;
}

resultado_t comunicar_join(uint32_t tid_llamador, uint32_t tid_esperador)
{
	loggear_debug("Comunico join [TID llamador: %d, TID esperador %d]",
		tid_llamador, tid_esperador);

	pedido_join_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = JOIN;
	cuerpo_del_mensaje.tid_llamador = tid_llamador;
	cuerpo_del_mensaje.tid_esperador = tid_esperador;

	char* chorro_de_envio = serializar_pedido_join_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_join_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

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
	loggear_debug("Comunico bloquear [PID: %d, ID del recurso %d]", tcb->pid,
		id_recurso);

	pedido_bloquear_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = BLOQUEAR;
	cuerpo_del_mensaje.tcb = tcb;
	cuerpo_del_mensaje.identificador_de_recurso = id_recurso;

	char* chorro_de_envio = serializar_pedido_bloquear_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_bloquear_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

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
	loggear_debug("Comunico despertar [PID: %d, ID del recurso: %d]", tcb->pid,
		id_recurso);

	pedido_despertar_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = DESPERTAR;
	cuerpo_del_mensaje.identificador_de_recurso = id_recurso;

	char* chorro_de_envio = serializar_pedido_despertar_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_despertar_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

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
