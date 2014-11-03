/*
 * memoria.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

// TODO arreglar con memoria si se va a mandar un mensaje mensaje al conectarse
#include "sockets.h"

#define PID_KERNEL 1;

sock_t* memoria;
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

	loggear_info("Envio con exito paquete de %d bytes", len_a_enviar);

	char* mensaje_recibido;
	uint32_t len_devolucion;

	loggear_trace("Me preparo para recibir respuesta");

	if (recibir(socket, &mensaje_recibido, &len_devolucion) == -1)
	{
		loggear_warning("No pudo recibir respuesta");
		return FALLO_COMUNICACION;
	}

	loggear_info("Recibio con exito paquete de %d bytes", len_devolucion);

	memcpy(respuesta, mensaje_recibido, len_devolucion);

	return OK;
}

resultado_t _conectar(sock_t** socket, char* ip, uint32_t puerto)
{
	loggear_trace("Intento conectarme con ip: %s y puerto: %d", ip, puerto);

	*socket = crear_socket_hablador(ip, puerto);

	if (conectar(*socket) == -1)
	{
		loggear_warning("No pudo conectarse a ip: %s y puerto: %d", ip, puerto);
		return FALLO_CONEXION;
	}

	loggear_info("Conexion realizada con exito al ip: %s y puerto: %d", ip,
		puerto);

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

	loggear_info("Comunicacion con kernel realizada con exito");

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

	loggear_info("Respuesta recibida correcta");

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

	if (_conectar(&memoria, ip_msp(), puerto_msp()) == FALLO_CONEXION)
		return FALLO_CONEXION;

	loggear_info("Conexion con memoria realizada con exito");

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

	loggear_info("Envio de mensaje de DESCONEXION_CPU realizado con exito");

	free(chorro_a_enviar);

	return OK;
}

resultado_t desconectar_memoria()
{
	loggear_trace("Intento desconectarme de memoria");
	cerrar_liberar(memoria);
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
	desconectar_kernel();

	return OK;
}

resultado_t crear_segmento(direccion pid, uint32_t bytes, direccion* direccion)
{
	pedido_de_crear_segmento_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = CREAME_UN_SEGMENTO;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.tamano = bytes;

	char* chorro_de_envio = serializar_pedido_de_crear_segmento_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(
		tamanio_respuesta_de_crear_segmento_t_serializado());

	if (_enviar_y_recibir(memoria, chorro_de_envio,
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
		free(respuesta);
		return FALLO_CREACION_DE_SEGMENTO;
	}

	*direccion = respuesta->direccion_virtual;

	free(respuesta);

	return OK;
}

resultado_t destruir_segmento(direccion pid, direccion direccion)
{
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
		free(respuesta);
		return FALLO_DESTRUCCION_DE_SEGMENTO;
	}

	free(respuesta);

	return OK;
}

resultado_t leer_de_memoria(direccion pid, direccion direccion,
	uint32_t cantidad_de_bytes, char* buffer)
{
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
		free(respuesta->bytes_leido);
		free(respuesta);
		return FALLO_LECTURA_DE_MEMORIA;
	}

	memcpy(buffer, respuesta->bytes_leido, cantidad_de_bytes);

	free(respuesta->bytes_leido);
	free(respuesta);

	return OK;
}

resultado_t escribir_en_memoria(direccion pid, direccion direccion,
	uint32_t cantidad_de_bytes, char* bytes_a_escribir)
{
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
		free(respuesta);
		return FALLO_ESCRITURA_EN_MEMORIA;
	}

	free(respuesta);

	return OK;
}

resultado_t pedir_tcb(tcb_t* tcb, int32_t* quantum)
{
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

	return OK;
}

resultado_t informar_a_kernel_de_finalizacion(tcb_t tcb, resultado_t res)
{
	if (res == EXCEPCION_POR_INTERRUPCION)
	{
		int32_t direccion;
		leer_numero(&tcb, &direccion);

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
			return FALLO_INFORME_A_KERNEL;
		}

		return OK;
	}

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
		return FALLO_INFORME_A_KERNEL;
	}

	return OK;
}

resultado_t _obtener(tcb_t* tcb, char* memoria_a_actualizar,
	uint32_t bytes_a_leer)
{
	uint32_t pid_a_leer = PID_KERNEL
	;

	if (!es_tcb_kernel(tcb))
		pid_a_leer = tcb->pid;

	if (leer_de_memoria(pid_a_leer, tcb->pc, bytes_a_leer, memoria_a_actualizar)
		== FALLO_LECTURA_DE_MEMORIA)
		return FALLO_LECTURA_DE_MEMORIA;

	actualizar_pc(tcb, tcb->pc + bytes_a_leer);

	return OK;
}

resultado_t leer_proxima_instruccion(tcb_t* tcb, instruccion_t instruccion)
{
	if (_obtener(tcb, instruccion, sizeof(instruccion_t) - 1)
		== FALLO_LECTURA_DE_MEMORIA)
		return FALLO_LECTURA_DE_MEMORIA;

	instruccion[4] = '\0';

	return OK;
}

resultado_t leer_registro(tcb_t* tcb, char* registro)
{
	if (_obtener(tcb, registro, sizeof(char)) == FALLO_LECTURA_DE_MEMORIA)
		return FALLO_LECTURA_DE_MEMORIA;

	return OK;
}

resultado_t leer_numero(tcb_t* tcb, int32_t* numero)
{
	char buffer[3];

	if (_obtener(tcb, buffer, sizeof(int32_t)) == FALLO_LECTURA_DE_MEMORIA)
		return FALLO_LECTURA_DE_MEMORIA;

	unir_bytes(numero, buffer);

	return OK;
}

resultado_t comunicar_entrada_estandar(tcb_t* tcb, uint32_t bytes_a_leer,
	uint32_t* bytes_leidos, char* buffer, idetificador_tipo_t identificador)
{
	pedido_entrada_estandar_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.pid = tcb->pid;
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

	*bytes_leidos = respuesta->tamanio;
	buffer = respuesta->cadena;

	free(chorro_de_envio);
	free(chorro_de_respuesta);
	free(respuesta->cadena);
	free(respuesta);

	return OK;
}

resultado_t comunicar_salida_estandar(tcb_t* tcb, uint32_t bytes_a_enviar,
	char* buffer)
{
	pedido_salida_estandar_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.pid = tcb->pid;
	cuerpo_del_mensaje.tamanio = bytes_a_enviar;
	cuerpo_del_mensaje.cadena_de_texto = buffer;

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
		return ERROR_EN_EJECUCION;

	return OK;
}

resultado_t comunicar_nuevo_tcb(tcb_t* nuevo_tcb)
{
	pedido_crear_hilo_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.tcb = nuevo_tcb;

	char* chorro_de_envio = serializar_pedido_crear_hilo_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(sizeof(resultado_t));

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_crear_hilo_t_serializado(), chorro_de_respuesta)
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
		return ERROR_EN_EJECUCION;

	return OK;
}

resultado_t comunicar_join(uint32_t tid_llamador, uint32_t tid_esperador)
{
	pedido_join_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
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
		return ERROR_EN_EJECUCION;

	return OK;
}

resultado_t comunicar_bloquear(tcb_t* tcb, uint32_t id_recurso)
{
	pedido_bloquear_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
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
		return ERROR_EN_EJECUCION;

	return OK;
}

resultado_t comunicar_despertar(tcb_t* tcb, uint32_t id_recurso)
{
	pedido_despertar_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
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
		return ERROR_EN_EJECUCION;

	return OK;
}

// TODO avisar a mati que tiene que implementar las serializaciones
resultado_t pedir_tid_a_kernel(tcb_t tcb, direccion* nuevo_tid)
{
	pedido_nuevo_tid_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = DAME_TID;
	cuerpo_del_mensaje.tcb = &tcb;

	char* chorro_de_envio = serializar_pedido_nuevo_tid_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(
		tamanio_respuesta_nuevo_tid_t_serializado());

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_nuevo_tid_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION)
	{
		free(chorro_de_envio);
		free(chorro_de_respuesta);
		return FALLO_COMUNICACION;
	}

	respuesta_nuevo_tid_t* respuesta = deserializar_respuesta_nuevo_tid_t(
		chorro_de_respuesta);

	*nuevo_tid = respuesta->tid;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	return OK;
}

// TODO eliminar (ya no se hace falta)
//void pedir_al_kernel_tamanio_stack(uint32_t *tamano_stack)
//{
//
//}
