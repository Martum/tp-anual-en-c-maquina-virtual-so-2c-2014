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


uint32_t ID_CPU_GLOBAL = 1;

uint32_t dame_nuevo_id_cpu()
{
	return ID_CPU_GLOBAL++;
}

void copiar_encabezado(tcb_t* to, tcb_t* from)
{
	to->pid = from->pid;
	to->tid = from->tid;
}

void copiar_registros_programacion(tcb_t* to, tcb_t* from)
{
	to->a = from->a;
	to->b = from->b;
	to->c = from->c;
	to->d = from->d;
	to->e = from->e;
	to->f = from->f;
}

void copiar_tcb(tcb_t* to, tcb_t* from)
{
	copiar_encabezado(to, from);

	to->base_codigo = from->base_codigo;
	to->tamanio_codigo = from->tamanio_codigo;

	to->pc = from->pc;

	to->base_stack = from->base_stack;
	to->cursor_stack = from->cursor_stack;

	copiar_registros_programacion(to, from);
}

// TODO: Esto hay que verlo con Santi
int crear_hilo(tcb_t* tcb)
{
	// Creamos TCB
	tcb_t* tcb_hilo = crear_tcb(tcb->pid, dame_nuevo_tid());

	// Le asignamos la misma base_codigo y un nuevo base_stack
	if(cargar_tcb_sin_codigo(tcb, tcb_hilo) != -1)
	{// Hay memoria, agregamos a RDY
		agregar_a_ready(tcb_hilo);
		return 0;
	}

	return -1;
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

int recibir_entrada_estandar(respuesta_entrada_estandar_t* entrada_estandar)
{
	// Buscamos el ID del CPU (NO SE HACE FREE)
	ejecutando_t* ej = buscar_exec_por_pid_tid(entrada_estandar->pid, entrada_estandar->tid);

	// Buscamos socket del CPU (NO SE HACE FREE)
	sock_t* socket_cpu = buscar_conexion_cpu_por_id(ej->cpu);

	// Enviamos la respuesta entrada serializada
	char* mensaje = serializar_respuesta_entrada_estandar_t(entrada_estandar);
	uint32_t len = tamanio_respuesta_entrada_estandar_t_serializado(entrada_estandar->tamanio);
	enviar(socket_cpu, mensaje, &len);

	free(mensaje);
	return 0;
}

int enviar_entrada_estandar(pedido_entrada_estandar_t* entrada_estandar)
{
	sock_t* conexion_proceso = buscar_conexion_proceso_por_pid(entrada_estandar->pid);
	char* pedido_serializado = serializar_pedido_entrada_estandar_t(entrada_estandar);
	uint32_t len = tamanio_pedido_entrada_estandar_t_serializado();

	int32_t res = enviar(conexion_proceso, pedido_serializado, &len);

	free(pedido_serializado);
	return res;
}

int salida_estandar(pedido_salida_estandar_t* pedido_salida)
{
	// Obtenemos socket proceso, serializamos el pedido
	sock_t* conexion_proceso = buscar_conexion_proceso_por_pid(pedido_salida->pid);
	char* salida_serializada = serializar_pedido_salida_estandar_t(pedido_salida);
	uint32_t len = tamanio_pedido_salida_estandar_t_serializado(pedido_salida->tamanio);

	// Enviamos pedido
	int32_t res = enviar(conexion_proceso, salida_serializada, &len);

	// Free & return
	free(salida_serializada);
	return res;
}


void bloquear(uint32_t recurso)
{
	// Este TCB es el que ejecuto la syscall (actualmente la esta corriendo el TCB KM)
	tcb_t* tcb = get_bloqueado_conclusion_tcb();

	agregar_a_block_recurso(tcb);
	agregar_a_cola_recurso(recurso, tcb);
}

void despertar(uint32_t recurso)
{
	tcb_t* tcb = quitar_primero_de_cola_recurso(recurso);
	quitar_de_block_recurso(tcb);
	agregar_a_ready(tcb);
}

void interrupcion(tcb_t* tcb, direccion dir)
{
	// Quitamos de EXEC y copiamos el nuevo estado
	tcb_t* tcb_actual = quitar_de_exec(tcb);
	copiar_tcb(tcb_actual, tcb);

	if(hay_hilos_block_espera_km() || tcb_km_is_running())
	{
		// Encolamos el struct a la espera del KM
		esperando_km_t* ekm = malloc(sizeof(esperando_km_t));
		ekm->tcb = tcb_actual;
		ekm->direccion_syscall = dir;

		agregar_a_block_espera_km(ekm);
	}
	else
	{// TCB KM libre, lo podemos usar
		preparar_km_para_ejecutar(tcb_actual, dir);
	}

	// NO HACER FREE DE NADA
}

void join(uint32_t llamador, uint32_t esperando_a)
{
	esperando_join_t* ej = malloc(sizeof(esperando_join_t));
	ej->tcb = get_bloqueado_conclusion_tcb();
	ej->esperando_a = esperando_a;

	agregar_a_block_join(ej);

	set_enviar_a_rdy(false);
}

void preparar_km_para_ejecutar(tcb_t* tcb, direccion direccion)
{
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

