/*
 * memoria.c
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

// TODO arreglar con MSP los mensajes de errores y validar mensajes de envio y respuesta
#include "sockets.h"

sock_t* memoria;
sock_t* kernel;

resultado_t _enviar_y_recibir(sock_t* socket, char* chorro_a_enviar,
	uint32_t len_a_enviar, char* respuesta)
{

	// ENVIO EL MENSAJE
	if (enviar(socket, chorro_a_enviar, &len_a_enviar) == -1)
		return FALLO_COMUNICACION;

	// PREPARO LA RESPUESTA
	char* mensaje_recibido;
	uint32_t len_devolucion;

	// RECIBO LA RESPUESTA
	if (recibir(socket, &mensaje_recibido, &len_devolucion) == -1)
		return FALLO_COMUNICACION;

	// DEVUELVE LA RESPUESTA
	memcpy(respuesta, mensaje_recibido, len_devolucion);

	return OK;
}

resultado_t _conectar(sock_t** socket, char* ip, uint32_t puerto)
{
	printf("%s", ip);
	*socket = crear_socket_hablador(ip, puerto);
	if (conectar(*socket) == -1)
		return FALLO_CONEXION;
	return OK;
}

resultado_t conectar_con_memoria()
{
	return _conectar(&memoria, ip_msp(), puerto_msp());
}

resultado_t _mandar_soy_cpu_a_kernel()
{
	pedido_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = SOY_CPU;

	char* chorro_de_envio = serializar_pedido_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_pedido_t_serializado());

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION) {

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		return FALLO_COMUNICACION;
	}

	pedido_t respuesta = *deserializar_pedido_t(chorro_de_respuesta);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	if (respuesta.flag != BIENVENIDO) {
		return FALLO_COMUNICACION;
	}

	return OK;
}

resultado_t conectar_con_kernel()
{
	if (_conectar(&kernel, ip_kernel(), puerto_kernel()) == FALLO_CONEXION)
		return FALLO_CONEXION;

	if (_mandar_soy_cpu_a_kernel() == FALLO_COMUNICACION)
		return FALLO_CONEXION;

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
		== FALLO_COMUNICACION) {

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		return FALLO_PEDIDO_DE_TCB;
	}

	respuesta_de_nuevo_tcb_t respuesta = *deserializar_respuesta_de_nuevo_tcb_t(
		chorro_de_respuesta);
	*tcb = *respuesta.tcb;
	*quantum = respuesta.quantum;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

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
		== FALLO_COMUNICACION) {

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		return FALLO_CREACION_DE_SEGMENTO;
	}

	respuesta_de_crear_segmento_t respuesta =
		*deserializar_respuesta_de_crear_segmento_t(chorro_de_respuesta);

	*direccion = respuesta.direccion_virtual;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	return OK;
}

// TODO agregar lectura de respuesta
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
		chorro_de_respuesta) == FALLO_COMUNICACION) {

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		return FALLO_DESTRUCCION_DE_SEGMENTO;
	}

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	return OK;
}

resultado_t leer_de_memoria(direccion pid, direccion direccion, uint32_t bytes,
	char* buffer)
{
	pedido_de_leer_de_memoria_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = LEE_DE_MEMORIA;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;
	cuerpo_del_mensaje.tamano = bytes;

	char* chorro_de_envio = serializar_pedido_de_leer_de_memoria_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(
		tamanio_respuesta_de_leer_de_memoria_t_serializado());

	if (_enviar_y_recibir(memoria, chorro_de_envio,
		tamanio_pedido_de_leer_de_memoria_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION) {

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		return FALLO_LECTURA_DE_MEMORIA;
	}

	respuesta_de_leer_de_memoria_t respuesta =
		*deserializar_respuesta_de_leer_de_memoria_t(chorro_de_respuesta);

	memcpy(buffer, respuesta.bytes_leido, bytes);

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	return OK;
}

// TODO agregar lectura de respuesta
resultado_t escribir_en_memoria(direccion pid, direccion direccion,
	uint32_t bytes, char* buffer)
{
	pedido_de_escribir_en_memoria_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = ESCRIBI_EN_MEMORIA;
	cuerpo_del_mensaje.pid = pid;
	cuerpo_del_mensaje.direccion_virtual = direccion;
	cuerpo_del_mensaje.bytes_a_escribir = buffer;
	cuerpo_del_mensaje.tamano = bytes;

	char* chorro_de_envio = serializar_pedido_de_escribir_en_memoria_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_respuesta_t_serializado());

	if (_enviar_y_recibir(memoria, chorro_de_envio,
		tamanio_pedido_de_escribir_en_memoria_t_serializado(),
		chorro_de_respuesta) == FALLO_COMUNICACION) {

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		return FALLO_ESCRITURA_EN_MEMORIA;
	}

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	return OK;
}

// TODO agregar lectura de respuesta
resultado_t informar_a_kernel_de_finalizacion(tcb_t tcb, resultado_t res)
{
	pedido_con_resultado_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.tcb = &tcb; // TODO no estoy seguro de que esto funcione
	cuerpo_del_mensaje.resultado = res;

	char* chorro_de_envio = serializar_pedido_con_resultado_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_respuesta_t_serializado());

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_con_resultado_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION) {

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		return FALLO_INFORME_A_KERNEL;
	}

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	return OK;
}

void cerrar_puertos()
{
	cerrar_liberar(memoria);
	cerrar_liberar(kernel); // todo agregar mensaje de DESCONEXION_CPU
}

void _obtener(tcb_t* tcb, char* memoria_a_actualizar, uint32_t bytes_a_leer)
{
	leer_de_memoria(tcb->pid, tcb->pc, bytes_a_leer, memoria_a_actualizar);
	tcb->pc = tcb->pc + bytes_a_leer;
}

void obtener_instruccion(tcb_t* tcb, instruccion_t instruccion)
{
	_obtener(tcb, instruccion, sizeof(instruccion_t));
	instruccion[4] = '\0';
}

void obtener_registro(tcb_t* tcb, char* registro)
{
	_obtener(tcb, registro, sizeof(char));
}

void obtener_numero(tcb_t* tcb, int32_t* numero)
{
	char buffer[3];
	_obtener(tcb, buffer, sizeof(int32_t));
	unir_bytes(numero, buffer);
}

// TODO arreglar con kernel este mensaje
void pedir_al_kernel_tamanio_stack(uint32_t* tamanio_stack)
{
	// todo programar
}

// TODO agregar identificador de tipo
resultado_t comunicar_entrada_estandar(tcb_t* tcb, uint32_t bytes_leidos,
	char* buffer)
{
	pedido_entrada_estandar_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.pid = tcb->pid;
//	cuerpo_del_mensaje.identificador_de_tipo =

	char* chorro_de_envio = serializar_pedido_entrada_estandar_t(
		&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(
		tamanio_respuesta_entrada_estandar_t_serializado());

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_entrada_estandar_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION) {

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		return FALLO_COMUNICACION;
	}

	respuesta_entrada_estandar_t respuesta =
		*deserializar_respuesta_entrada_estandar_t(chorro_de_respuesta);

	buffer = respuesta.cadena;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	return OK;
}

// TODO avisar a kernel que tiene que devolver un OK
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
	char* chorro_de_respuesta = malloc(tamanio_respuesta_t_serializado());

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_salida_estandar_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION) {

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		return FALLO_COMUNICACION;
	}

	respuesta_t respuesta = *deserializar_respuesta_t(chorro_de_respuesta);

	if (respuesta.resultado != OK)
		return ERROR_EN_EJECUCION;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	return OK;
}

// TODO avisar a kernel que tiene que devolver un OK
resultado_t comunicar_nuevo_tcb(tcb_t* nuevo_tcb)
{
	pedido_crear_hilo_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.tcb = nuevo_tcb;

	char* chorro_de_envio = serializar_pedido_crear_hilo_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_respuesta_t_serializado());

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_crear_hilo_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION) {

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		return FALLO_COMUNICACION;
	}

	respuesta_t respuesta = *deserializar_respuesta_t(chorro_de_respuesta);

	if (respuesta.resultado != OK)
		return ERROR_EN_EJECUCION;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	return OK;
}

resultado_t comunicar_join(uint32_t tid_llamador, uint32_t tid_esperador)
{
	pedido_join_t cuerpo_del_mensaje;
	cuerpo_del_mensaje.flag = TOMA_RESULTADO;
	cuerpo_del_mensaje.tid_llamador = tid_llamador;
	cuerpo_del_mensaje.tid_esperador = tid_esperador;

	char* chorro_de_envio = serializar_pedido_join_t(&cuerpo_del_mensaje);
	char* chorro_de_respuesta = malloc(tamanio_respuesta_t_serializado());

	if (_enviar_y_recibir(kernel, chorro_de_envio,
		tamanio_pedido_join_t_serializado(), chorro_de_respuesta)
		== FALLO_COMUNICACION) {

		free(chorro_de_envio);
		free(chorro_de_respuesta);

		return FALLO_COMUNICACION;
	}

	respuesta_t respuesta = *deserializar_respuesta_t(chorro_de_respuesta);

	if (respuesta.resultado != OK)
		return ERROR_EN_EJECUCION;

	free(chorro_de_envio);
	free(chorro_de_respuesta);

	return OK;
	return OK;
}

resultado_t comunicar_bloquear(tcb_t* tcb, uint32_t id_recurso)
{
	// todo programar
	return OK;
}

resultado_t comunicar_despertar(tcb_t* tcb, uint32_t id_recurso)
{
	// todo programar
	return OK;
}

// TODO eliminar porque ya no sirve mas
/*
 resultado_t _enviar_y_recibir_sin_serializacion(sock_t* socket,
 void* cuerpo_del_mensaje, uint32_t tamano, void* respuesta)
 {
 // ARMO EL CHORRO DE BYTES
 char* mensaje_a_enviar = malloc(tamano);
 uint32_t len_a_enviar = tamano;
 memcpy(mensaje_a_enviar, cuerpo_del_mensaje, len_a_enviar);

 // ENVIO EL MENSAJE
 if (enviar(socket, mensaje_a_enviar, &len_a_enviar) == -1)
 return FALLO_COMUNICACION;

 // PREPARO LA RESPUESTA
 char* mensaje_recibido;
 uint32_t len_devolucion;

 // RECIBO LA RESPUESTA
 if (recibir(socket, &mensaje_recibido, &len_devolucion) == -1)
 return FALLO_COMUNICACION;

 // DEVUELVE LA RESPUESTA
 memcpy(respuesta, mensaje_recibido, len_devolucion);
 return OK;
 }

 resultado_t pedir_tcb_sin_serializacion(tcb_t* tcb, int32_t* quantum)
 {
 pedido_t cuerpo_del_mensaje;
 respuesta_de_nuevo_tcb_t respuesta;
 cuerpo_del_mensaje.flag = MANDA_TCB;

 if (_enviar_y_recibir_sin_serializacion(kernel, &cuerpo_del_mensaje,
 sizeof(pedido_t), &respuesta) == FALLO_COMUNICACION) {
 return FALLO_PEDIDO_DE_TCB;
 }

 *tcb = *respuesta.tcb;
 *quantum = respuesta.quantum;

 return OK;
 }

 resultado_t crear_segmento_sin_serializacion(direccion pid, uint32_t bytes,
 direccion* direccion)
 {
 pedido_de_crear_segmento_t cuerpo_del_mensaje;
 respuesta_de_crear_segmento_t respuesta;
 cuerpo_del_mensaje.flag = CREAME_UN_SEGMENTO;
 cuerpo_del_mensaje.pid = pid;
 cuerpo_del_mensaje.tamano = bytes;

 if (_enviar_y_recibir_sin_serializacion(memoria, &cuerpo_del_mensaje,
 sizeof(pedido_de_crear_segmento_t), &respuesta) == FALLO_COMUNICACION) {
 return FALLO_CREACION_DE_SEGMENTO;
 }

 *direccion = respuesta.direccion_virtual;

 return OK;
 }

 resultado_t destruir_segmento_sin_serializacion(direccion pid,
 direccion direccion)
 {
 pedido_de_destruir_segmento_t cuerpo_del_mensaje;
 respuesta_t respuesta;
 cuerpo_del_mensaje.flag = DESTRUI_SEGMENTO;
 cuerpo_del_mensaje.pid = pid;
 cuerpo_del_mensaje.direccion_virtual = direccion;

 return _enviar_y_recibir_sin_serializacion(memoria, &cuerpo_del_mensaje,
 sizeof(pedido_de_destruir_segmento_t), &respuesta);
 }

 resultado_t leer_de_memoria_sin_serializacion(direccion pid,
 direccion direccion, uint32_t bytes, void* buffer)
 {
 pedido_de_leer_de_memoria_t cuerpo_del_mensaje;
 respuesta_de_leer_de_memoria_t respuesta;
 cuerpo_del_mensaje.flag = LEE_DE_MEMORIA;
 cuerpo_del_mensaje.pid = pid;
 cuerpo_del_mensaje.direccion_virtual = direccion;
 cuerpo_del_mensaje.tamano = bytes;

 if (_enviar_y_recibir_sin_serializacion(memoria, &cuerpo_del_mensaje,
 sizeof(pedido_de_leer_de_memoria_t), &respuesta))
 return FALLO_LECTURA_DE_MEMORIA;

 memcpy(buffer, respuesta.bytes_leido, sizeof(respuesta.bytes_leido));

 return OK;
 }

 resultado_t escribir_en_memoria_sin_serializacion(direccion pid,
 direccion direccion, uint32_t bytes, void* buffer)
 {
 pedido_de_escribir_en_memoria_t cuerpo_del_mensaje;
 respuesta_t respuesta;
 cuerpo_del_mensaje.flag = ESCRIBI_EN_MEMORIA;
 cuerpo_del_mensaje.pid = pid;
 cuerpo_del_mensaje.direccion_virtual = direccion;
 cuerpo_del_mensaje.bytes_a_escribir = (char*) buffer;
 cuerpo_del_mensaje.tamano = bytes;

 return _enviar_y_recibir_sin_serializacion(memoria, &cuerpo_del_mensaje,
 sizeof(pedido_de_escribir_en_memoria_t), &respuesta);
 }

 resultado_t informar_a_kernel_de_finalizacion_sin_serializacion(tcb_t tcb,
 resultado_t res)
 {
 pedido_con_resultado_t cuerpo_del_mensaje;
 respuesta_t respuesta;
 cuerpo_del_mensaje.flag = TOMA_RESULTADO;
 cuerpo_del_mensaje.tcb = &tcb;
 cuerpo_del_mensaje.resultado = res;

 return _enviar_y_recibir_sin_serializacion(kernel, &cuerpo_del_mensaje,
 sizeof(pedido_con_resultado_t), &respuesta);
 }
 */
