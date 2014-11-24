/*
 * serviciosacpu.h
 *
 *  Created on: 15/9/2014
 *      Author: martin
 */

#ifndef SERVICIOSACPU_H_
#define SERVICIOSACPU_H_

#include <hu4sockets/tcb.h>
#include <stdint.h>
#include <hu4sockets/mensajes.h>
#include <hu4sockets/sockets.h>
#include <hu4sockets/tcb.h>
#include "conexiones.h"
#include <stdlib.h>
#include "lstestados.h"


/**
 * Genera un nuevo ID para CPU
 */
uint32_t dame_nuevo_id_cpu();

/**
 * Copia el TID y el PID
 */
void copiar_encabezado(tcb_t* to, tcb_t* from);

/**
 * Copia los Registros de Programacion (A, B, C, D, E, F)
 */
void copiar_registros_programacion(tcb_t* to, tcb_t* from);

/**
 * Copia todos los elementos de FROM a TO
 */
void copiar_tcb(tcb_t* to, tcb_t* from);

/**
 * Destruye todos los segmentos de un proceso y los remueve de la lista correspondiente
 */
void destruir_segmentos_de_proceso(uint32_t pid);

/**
 * Crea un nuevo Hilo a partir de TCB y lo asigna a la cola de RDY.
 * Devuelve 0 en caso de exito o -1 en caso de no haber memoria disponible.
 */
int crear_hilo(tcb_t* tcb);


/**
 * Recibe un pid y un identificador de tipo y pide a la consola que ingrese un dato segun
 * el identificador.
 */
//void enviar_entrada_estandar(uint32_t pid, uint16_t identificador_tipo);


/**
 * recibe la respuesta de la consola.
 */
//void* recibir_entrada_estandar(uint32_t pid, void* );


/**
 * Envia al CPU la respuesta del Proceso a la entrada estandar.
 * Devuelve 0 en caso de exito; -1 si fallo
 */
int recibir_entrada_estandar(respuesta_entrada_estandar_t* entrada_estandar);

/**
 * Envia al Proceso un pedido de entrada estandar.
 * Devuelve 0 en caso de exito; -1 si falla.
 */
int enviar_entrada_estandar(pedido_entrada_estandar_t* entrada_estandar);

/**
 * Recibe un pedido_salida_estandar_t*. Escribe en la consola correspondiente al pid la cadena recibida.
 *  Retorna 0 en caso de exito o -1 en caso de error. (Puede haber error? lo unico que se me ocurre es que se
 * desconecte la consola, pero eso mataria al proceso directamente).
 */
int salida_estandar(pedido_salida_estandar_t* pedido_salida);

/**
 * Envia el hilo a block, lo encola en la cola de espera del recurso.
 */
void bloquear(uint32_t recurso);

/**
 * Remueve el primer hilo de la cola de espera del recurso y lo manda a ready.
 */
void despertar(uint32_t recurso);

/**
 * Si esta el tcb km disponible, proceder normalmente, si no lo està, tengo qe agregar el tcb qe recibo en la cola de
 * block_esperando_km y devolverle otro tcb cualquiera. Al liberarse el tcb km, revisar si hay tcb esperandolo, si lo hay
 * mandarlos a procesar, sino seguir normalmente.
 */
void interrupcion(tcb_t* tcb, direccion dir);

/**
 * Encola al TCB llamador en block join a la espera de que el TCB esperando_a termine.
 */
void join(uint32_t llamador, uint32_t esperando_a);

/**
 * Prepara el TCB KM para ejecutarlo. Lo encola en rdy[0] y deja
 * al TCB usuario en block_conclusion_km
 */
void preparar_km_para_ejecutar(tcb_t* tcb, direccion direccion);


typedef struct segmentos_por_hilo {
	t_list* segmentos;
	uint32_t pid;
	uint32_t tid;
} segmentos_por_hilo_t;

/*
 * Agrega un segmentos_por_hilo_t* a la lista de segmentos por hilo.
 * En caso que la lista sea NULL, la inicializa.
 */
void agregar_segmentos_por_hilo(segmentos_por_hilo_t* segmentos);

/*
 * Quita el segmento que se corresponde con la direccion virtual recibida,
 * del hilo correspondiente.
 */
void quitar_segmento_de_hilo(uint32_t pid, uint32_t tid, direccion dir_virtual);

void crear_segmento_cpu(uint32_t pid, uint32_t tamanio, uint32_t* cpu_id);

void destruir_segmento_cpu(uint32_t pid, direccion dir_virtual, uint32_t* cpu_id);

segmentos_por_hilo_t* find_segmento_de_hilo(uint32_t pid, uint32_t tid);

#endif /* SERVICIOSACPU_H_ */
