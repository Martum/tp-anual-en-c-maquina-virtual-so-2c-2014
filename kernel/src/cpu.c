/*
 * serviciosacpu.c
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#include "loader.h"
#include "lstestados.h"
#include "consola.h"
#include "cpu.h"
#include "memoria.h"
#include "configuraciones.h"

uint32_t ID_CPU_GLOBAL = 1;

// lista de segmentos por hilo (struct: pid, tid, lista de direcciones(segmentos))
t_list* SEGMENTOS_POR_HILO = NULL;

void inicializar_lista_segmentos_por_hilo()
{
	SEGMENTOS_POR_HILO = list_create();
}

void agregar_segmentos_por_hilo(segmentos_por_hilo_t* segmentos)
{
	list_add(SEGMENTOS_POR_HILO, segmentos);
}

// TODO: recordar hacer malloc para las direcciones cuando las cree.
void quitar_segmento_de_hilo(uint32_t pid, uint32_t tid, direccion dir_virtual) {

	segmentos_por_hilo_t* segmentos_hilo = find_segmento_de_hilo(pid,tid);

	bool _segmentoQueQuiero(void* elemento) {
		return *((direccion*) elemento) == dir_virtual;
	}

	direccion* dir = list_remove_by_condition(segmentos_hilo->segmentos,
			_segmentoQueQuiero);
	free(dir);
}

segmentos_por_hilo_t* find_segmento_de_hilo(uint32_t pid, uint32_t tid) {

	bool _elementoConListaDeSegmentos(void* elemento) {
		return ((segmentos_por_hilo_t*) elemento)->pid == pid
				&& ((segmentos_por_hilo_t*) elemento)->tid == tid;
	}

	return list_find(SEGMENTOS_POR_HILO, _elementoConListaDeSegmentos);
}

void destruir_segmentos(t_list* lista, uint32_t pid)
{
	void _eliminar(void* elemento)
	{
		destruir_segmento(pid, *((direccion*)elemento));
		free(elemento);
	}

	list_clean_and_destroy_elements(lista, _eliminar);
}

void destruir_segmentos_de_proceso(uint32_t pid)
{
	bool _coincide_pid(void* elemento)
	{
		return ((segmentos_por_hilo_t*) elemento)->pid == pid;
	}

	uint32_t cantidad = list_count_satisfying(SEGMENTOS_POR_HILO, _coincide_pid);

	int i;
	for(i = 0; i < cantidad; i++)
	{
		segmentos_por_hilo_t* seg = list_remove_by_condition(SEGMENTOS_POR_HILO, _coincide_pid);

		destruir_segmentos(seg->segmentos, pid);

		list_destroy(seg->segmentos);
		free(seg);
	}
}

void destruir_segmentos_de_hilo(uint32_t pid, uint32_t tid)
{
	bool _coincide_pid_tid(void* elemento)
	{
		return ((segmentos_por_hilo_t*) elemento)->pid == pid
				&& ((segmentos_por_hilo_t*) elemento)->tid == tid;
	}

	segmentos_por_hilo_t* segmentos = list_remove_by_condition(SEGMENTOS_POR_HILO, _coincide_pid_tid);

	if(segmentos != NULL)
	{
		destruir_segmentos(segmentos->segmentos, pid);
		list_destroy(segmentos->segmentos);
		free(segmentos);
	}

}

void _enviar_rta_crear_segmento_a_cpu(char* rta_serializada, uint32_t tamanio, uint32_t* cpu_id) {

	sock_t* socket = buscar_conexion_cpu_por_id(*cpu_id);
	enviar(socket, rta_serializada, &tamanio);

	free(rta_serializada);
}

void crear_segmento_cpu(uint32_t pid, uint32_t tamanio, uint32_t* cpu_id) {
	direccion* dir = malloc(sizeof(direccion));
	uint32_t tid = get_tcb_km()->tid;

	uint32_t tamanio_rta = tamanio_respuesta_de_crear_segmento_t_serializado();
	char * rta_serializada = malloc(tamanio_rta);
	rta_serializada = crear_segmento_retornando_rta_serializada(pid, tamanio, dir);

	respuesta_de_crear_segmento_t* rta_deserializada = deserializar_respuesta_de_crear_segmento_t(rta_serializada);

	// TODO: REVISAR QUE LA REPSUESTA CORRECTA SEA RESULTADO_OK
	if (rta_deserializada->resultado == RESULTADO_OK) {
		segmentos_por_hilo_t* segmentos = find_segmento_de_hilo(pid,tid);
		if (segmentos == NULL){
			segmentos_por_hilo_t* segmentos = malloc(sizeof(segmentos_por_hilo_t));
			segmentos->pid = pid;
			segmentos->tid = tid;
			segmentos->segmentos = list_create();
		}
		list_add(segmentos->segmentos, dir);
	}

	_enviar_rta_crear_segmento_a_cpu(rta_serializada, tamanio_rta, cpu_id);
}

void _enviar_rta_destruir_segmento_a_cpu(char* rta_serializada, uint32_t tamanio, uint32_t* cpu_id) {

	sock_t* socket = buscar_conexion_cpu_por_id(*cpu_id);
	enviar(socket, rta_serializada, &tamanio);

	free(rta_serializada);
}

void destruir_segmento_cpu(uint32_t pid, direccion dir_virtual, uint32_t* cpu_id) {
	uint32_t tid = get_tcb_km()->tid;
	quitar_segmento_de_hilo(pid,tid,dir_virtual);

	uint32_t tamanio = tamanio_respuesta_t_serializado();
	char* rta_serializada = malloc(tamanio);
	rta_serializada = destruir_segmento_retornando_rta_serializada(pid, dir_virtual);

	_enviar_rta_destruir_segmento_a_cpu(rta_serializada, tamanio, cpu_id);
}

uint32_t dame_nuevo_id_cpu() {
	return ID_CPU_GLOBAL++;
}

void copiar_encabezado(tcb_t* to, tcb_t* from) {
	to->pid = from->pid;
	to->tid = from->tid;
}

void copiar_registros_programacion(tcb_t* to, tcb_t* from) {
	to->a = from->a;
	to->b = from->b;
	to->c = from->c;
	to->d = from->d;
	to->e = from->e;
	to->f = from->f;
}

void copiar_tcb(tcb_t* to, tcb_t* from) {
	copiar_encabezado(to, from);

	to->base_codigo = from->base_codigo;
	to->tamanio_codigo = from->tamanio_codigo;

	to->pc = from->pc;

	to->base_stack = from->base_stack;
	to->cursor_stack = from->cursor_stack;

	copiar_registros_programacion(to, from);
}

// TODO: Esto hay que verlo con Santi
int crear_hilo(tcb_t* tcb) {
	// Creamos TCB
	tcb_t* tcb_hilo = crear_tcb(tcb->pid, dame_nuevo_tid(tcb->pid));

	// Le asignamos la misma base_codigo y un nuevo base_stack
	if (cargar_tcb_sin_codigo(tcb, tcb_hilo) != -1) {// Hay memoria, agregamos a RDY
		agregar_a_ready(tcb_hilo);
		return 0;
	}

	return -1;
}




/*
 * 	@DESC:	Crea un stack para el nuevo_tcb y se lo asigna
 */
resultado_t _crear_stack(tcb_t* tcb)
{
	uint32_t tamano_stack = tamanio_stack();

	direccion nueva_base_stack;

	if (crear_segmento(tcb->pid, tamano_stack, &nueva_base_stack)
		== FALLO_CREACION_DE_SEGMENTO)
		return ERROR_EN_EJECUCION;

	tcb->base_stack = nueva_base_stack;

	return OK;
}

resultado_t _mover_cursor_stack(tcb_t* tcb, int32_t cantidad_de_bytes)
{
	if (tcb->base_stack > tcb->cursor_stack + cantidad_de_bytes) {
		return EXCEPCION_POR_POSICION_DE_STACK_INVALIDA;
	}

	tcb->cursor_stack = tcb->cursor_stack + cantidad_de_bytes;

	return OK;
}


/*
 * 	@DESC:	Copia todos los valores del stack del tcb al nuevo_tcb, actualizado los punteros.
 */
resultado_t _clonar_stack(tcb_t* nuevo_tcb, tcb_t* tcb)
{
	uint32_t cantidad_bytes_ocupado_stack = tcb->base_stack - tcb->cursor_stack;

	char* buffer = malloc(cantidad_bytes_ocupado_stack);

	if (leer_de_memoria(tcb->pid, tcb->base_stack, cantidad_bytes_ocupado_stack, buffer)
		== FALLO_LECTURA_DE_MEMORIA)
	{
		free(buffer);
		return ERROR_EN_EJECUCION;
	}

	if (escribir_memoria(nuevo_tcb->pid, nuevo_tcb->base_stack,
			buffer, cantidad_bytes_ocupado_stack) == -1)
	{
		free(buffer);
		return ERROR_EN_EJECUCION;
	}

	free(buffer);


	if (_mover_cursor_stack(tcb, cantidad_bytes_ocupado_stack)
		== EXCEPCION_POR_POSICION_DE_STACK_INVALIDA)
		return ERROR_EN_EJECUCION;

	return OK;
}



/*// NO GO
 void enviar_entrada_estandar(uint32_t pid, uint16_t identificador_tipo){
 //REVISAR. Utilizar el enviar y recibir para evitar espera activa.
 if (identificador_tipo == 1){
 pedido_leer_int(pid);
 }
 if (identificador_tipo == 2){
 pedido_leer_cadena(pid);
 }
 }

 // NO GO
 void* recibir_entrada_estandar(uint32_t pid, void* ){
 codear;
 }
 */

int recibir_entrada_estandar(respuesta_entrada_estandar_t* entrada_estandar) {
	// Buscamos el ID del CPU (NO SE HACE FREE)
	ejecutando_t* ej = buscar_exec_por_pid_tid(entrada_estandar->pid,
			entrada_estandar->tid);

	// Buscamos socket del CPU (NO SE HACE FREE)
	sock_t* socket_cpu = buscar_conexion_cpu_por_id(ej->cpu);

	// Enviamos la respuesta entrada serializada
	char* mensaje = serializar_respuesta_entrada_estandar_t(entrada_estandar);
	uint32_t len = tamanio_respuesta_entrada_estandar_t_serializado(
			entrada_estandar->tamanio);
	enviar(socket_cpu, mensaje, &len);

	free(mensaje);
	return 0;
}

int enviar_entrada_estandar(pedido_entrada_estandar_t* entrada_estandar) {
	sock_t* conexion_proceso = buscar_conexion_proceso_por_pid(
			entrada_estandar->pid);
	char* pedido_serializado = serializar_pedido_entrada_estandar_t(
			entrada_estandar);
	uint32_t len = tamanio_pedido_entrada_estandar_t_serializado();

	int32_t res = enviar(conexion_proceso, pedido_serializado, &len);

	free(pedido_serializado);
	return res;
}

int salida_estandar(pedido_salida_estandar_t* pedido_salida) {
	// Obtenemos socket proceso, serializamos el pedido
	sock_t* conexion_proceso = buscar_conexion_proceso_por_pid(
			pedido_salida->pid);
	char* salida_serializada = serializar_pedido_salida_estandar_t(
			pedido_salida);
	uint32_t len = tamanio_pedido_salida_estandar_t_serializado(
			pedido_salida->tamanio);

	// Enviamos pedido
	int32_t res = enviar(conexion_proceso, salida_serializada, &len);

	// Free & return
	free(salida_serializada);
	return res;
}

void bloquear(uint32_t recurso) {
	// Este TCB es el que ejecuto la syscall (actualmente la esta corriendo el TCB KM)
	tcb_t* tcb = get_bloqueado_conclusion_tcb();

	agregar_a_block_recurso(tcb);
	agregar_a_cola_recurso(recurso, tcb);

	set_enviar_a_rdy(false);
}

void despertar(uint32_t recurso) {
	tcb_t* tcb = quitar_primero_de_cola_recurso(recurso);
	quitar_de_block_recurso(tcb);
	agregar_a_ready(tcb);
}

respuesta_crear_hilo_t* _crear_hilo_desde_crea(tcb_t* tcb){
	direccion nuevo_tid = dame_nuevo_tid(tcb->pid);
	tcb_t* nuevo_tcb = crear_tcb(tcb->pid, nuevo_tid);
	copiar_tcb(nuevo_tcb, tcb);
	nuevo_tcb->tid = nuevo_tid;
	nuevo_tcb->pc = tcb->b;
	nuevo_tcb->km = 0;
	tcb->a = nuevo_tcb->tid;

	respuesta_crear_hilo_t* rta_crea = malloc(sizeof(rta_crea));
	rta_crea->nuevo_tid = nuevo_tid;

	if (_crear_stack(nuevo_tcb) == ERROR_EN_EJECUCION)
		rta_crea->resultado =  ERROR_EN_EJECUCION;
		return rta_crea;

	if (_clonar_stack(nuevo_tcb, tcb) == ERROR_EN_EJECUCION)
		rta_crea->resultado =  ERROR_EN_EJECUCION;
		return rta_crea;

	agregar_a_ready(nuevo_tcb);

	return rta_crea;
}

void _enviar_respuesta_crea_a_cpu(respuesta_crear_hilo_t* rta, uint32_t* cpu_id){

	rta->flag = CREAR_HILO;

	uint32_t tamanio = tamanio_respuesta_crear_hilo_t_serializado();
	char * rta_serializada = malloc(tamanio);
	rta_serializada = serializar_respuesta_crear_hilo_t(rta);

	free(rta);

	sock_t* socket = buscar_conexion_cpu_por_id(*cpu_id);
	enviar(socket, rta_serializada, &tamanio);

	free(rta_serializada);
}

void crea(tcb_t* tcb, uint32_t* cpu_id)
{
	respuesta_crear_hilo_t* rta_crea = _crear_hilo_desde_crea(tcb);
	_enviar_respuesta_crea_a_cpu(rta_crea, cpu_id);
}

void interrupcion(tcb_t* tcb, direccion dir) {
	// Quitamos de EXEC y copiamos el nuevo estado
	tcb_t* tcb_actual = quitar_de_exec(tcb);
	copiar_tcb(tcb_actual, tcb);

	if (hay_hilos_block_espera_km() || tcb_km_is_running()) {
		// Encolamos el struct a la espera del KM
		esperando_km_t* ekm = malloc(sizeof(esperando_km_t));
		ekm->tcb = tcb_actual;
		ekm->direccion_syscall = dir;

		agregar_a_block_espera_km(ekm);
	} else {	// TCB KM libre, lo podemos usar
		preparar_km_para_ejecutar(tcb_actual, dir);
	}

	// NO HACER FREE DE NADA
}

void join(uint32_t llamador, uint32_t esperando_a) {
	esperando_join_t* ej = malloc(sizeof(esperando_join_t));
	ej->tcb = get_bloqueado_conclusion_tcb();
	ej->esperando_a = esperando_a;

	agregar_a_block_join(ej);

	set_enviar_a_rdy(false);
}

void preparar_km_para_ejecutar(tcb_t* tcb, direccion direccion) {
	// Copiamos los registros al TCB KM
	tcb_t* tcb_km = get_tcb_km();
	copiar_encabezado(tcb_km, tcb);
	copiar_registros_programacion(tcb_km, tcb);

	tcb_km->pc = direccion;

	// Agregamos el TCB Usuario a block
	agregar_a_block_conclusion_km(tcb);

	// Agregamos el TCB KM a rdy
	agregar_a_ready(tcb_km);
}

