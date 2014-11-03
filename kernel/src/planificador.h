/*
 * planificador.h
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include <hu4sockets/mensajes.h>

// Retorna el respuesta_de_nuevo_tcb_t* ya serializado.
char* _rta_nuevo_tcb();

// RECIBE UN TCB DEL CPU Y HACE LO QUE CORRESPONDA SEGUN EL RESULTADO.
void recibir_tcb(resultado_t resultado, tcb_t* tcb);

// AGREGA UNA CPU EN LA COLA DE ESPERA DE TCB.
void agregar_a_cpu_en_espera_de_tcb(uint32_t cpu_id);

// QUITA UNA CPU DE LA COLA DE ESPERA DE TCB.
void* quitar_de_cpu_en_espera_de_tcb();

// ENCOLA LA CPU EN LA COLA DE ESPERAS DE TCB Y LLAMA A PLANIFICAR
void pedir_tcb(uint32_t cpu_id);

// SE ENCARGA DE OBTENER UN TCB EN CASO DE QUE HAYA UNA DISPONIBLE Y DARSELO A LA CPU CORRESPONDIENTE
void _planificar();

// ENVIA LA RTA CON EL TCB A LA CPU.
void _enviar_tcb_a_cpu(tcb_t* tcb, uint32_t* cpu_id);

/**
 * Mueve todos los TCBs de este Proceso a Exit
 */
void mover_tcbs_a_exit(uint32_t pid);


#endif /* PLANIFICADOR_H_ */
