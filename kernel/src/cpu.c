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
	return 0;
}

int enviar_entrada_estandar(pedido_entrada_estandar_t* entrada_estandar)
{
	sock_t* conexion_proceso = buscar_conexion_proceso_por_pid(entrada_estandar->pid);
	char* pedido_serializado = serializar_pedido_entrada_estandar_t(entrada_estandar);
	uint32_t len = tamanio_pedido_entrada_estandar_t_serializado();

	int32_t res = enviar(conexion_proceso, pedido_serializado, &len);

	free(pedido_serializado);
	return 0;
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


void bloquear(tcb_t* tcb, char* recurso){
	quitar_de_exec(tcb);
	agregar_a_block(tcb);
	agregar_a_cola_recurso(recurso, tcb);
}

void despertar(char* recurso){
	tcb_t* tcb = quitar_primero_de_cola_recurso(recurso);
	quitar_de_block(tcb);
	agregar_a_ready(tcb);
}



