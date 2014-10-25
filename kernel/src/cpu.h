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


/**
 * Genera un nuevo ID para CPU
 */
uint32_t dame_nuevo_id_cpu();

/**
 * Crea un nuevo Hilo a partir de TCB y lo asigna a la cola de RDY.
 * Devuelve 0 en caso de exito o -1 en caso de no haber memoria disponible.
 */
int crear_hilo(tcb_t* tcb);


/**
 * Recibe un pid y un identificador de tipo y pide a la consola que ingrese un dato segun
 * el identificador.
 */
void enviar_entrada_estandar(uint32_t pid, uint16_t identificador_tipo);


/**
 * recibe la respuesta de la consola.
 */
void* recibir_entrada_estandar(uint32_t pid, void* );


/**
 * Recibe un pedido_salida_estandar_t*. Escribe en la consola correspondiente al pid la cadena recibida.
 *  Retorna 0 en caso de exito o -1 en caso de error. (Puede haber error? lo unico que se me ocurre es que se
 * desconecte la consola, pero eso mataria al proceso directamente).
 */
int salida_estandar(pedido_salida_estandar_t* pedido_salida);

/*
 * Envia el hilo a block, lo encola en la cola de espera del recurso.
 */
void bloquear(tcb_t* tcb, char* recurso);

/*
 * Remueve el primer hilo de la cola de espera del recurso y lo manda a ready.
 */
void despertar(char* recurso);

/**
 * Si esta el tcb km disponible, proceder normalmente, si no lo està, tengo qe agregar el tcb qe recibo en la cola de
 * block_esperando_km y devolverle otro tcb cualquiera. Al liberarse el tcb km, revisar si hay tcb esperandolo, si lo hay
 * mandarlos a procesar, sino seguir normalmente.
 */
void interrupcion(tcb_t* tcb, direccion dir);

#endif /* SERVICIOSACPU_H_ */
