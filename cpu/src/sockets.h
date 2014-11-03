/*
 * memoria.h
 *
 *  Created on: 15/09/2014
 *      Author: utnso
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hu4sockets/mensajes.h"
#include "hu4sockets/sockets.h"

#include "configuraciones.h"
#include "definiciones.h"
#include "operaciones-con-bits.h"
#include "tcb-funciones.h"

/*
 * @DESC: Crea el socket sock_t y se conecta con el ip y puerto
 * 			que levanta del archivo de configuracion
 * @RETURNS:
 * 		OK si pudo conectarse
 * 		FALLO_CONEXION si hubo problemas al conectarse
 */
resultado_t conectar_con_memoria();

/*
 * @DESC: Crea el socket sock_t y se conecta con el ip y puerto
 * 			que levanta del archivo de configuracion
 * @RETURNS:
 * 		OK si pudo conectarse
 * 		FALLO_CONEXION si hubo problemas al conectarse
 */
resultado_t conectar_con_kernel();

/*
 * @DESC: Cierra el puerto con memoria
 * @RETURNS:
 * 		OK siempre
 */
resultado_t desconectar_memoria();

/*
 * @DESC: Cierra el puerto con kernel y le mando un mensaje de desconexion
 * @RETURNS:
 * 		OK siempre
 */
resultado_t desconectar_kernel();

/*
 * @DESC: Cierra los puertos de memoria y kernel
 * @RETURNS:
 * 		OK siempre
 */
resultado_t desconectarse();

/*
 * @DESC: Le pide a memoria que cree un nuevo segmento
 * @RETURNS:
 * 		OK si pudo conectarse y se creo el nuevo segemento
 * 		FALLO_CREACION_DE_SEGMENTO si hubo problemas al crear el segmento
 */
resultado_t crear_segmento(direccion pid, uint32_t bytes, direccion* direccion);

/*
 * @DESC: Le pide a memoria que destruya un segmento
 * @RETURNS:
 * 		OK si pudo conectarse y se destruyo el segmento
 * 		FALLO_DESTRUCCION_DE_SEGMENTO si hubo problemas al destruir el segmento
 */
resultado_t destruir_segmento(direccion pid, direccion direccion);

/*
 * @DESC: Le pide a memoria que le mande un chorro de datos desde una direccion y el resultado lo guarda en buffer
 * @RETURNS:
 * 		OK si pudo conectarse y trajo bien los datos
 * 		FALLO_LECTURA_DE_MEMORIA si hubo problemas al traer los datos
 */
resultado_t leer_de_memoria(direccion pid, direccion direccion, uint32_t bytes,
	char* buffer);

/*
 * @DESC: Le pide a memoria que guarde a partir de una direccion un chorro de datos
 * @RETURNS:
 * 		OK si pudo conectarse y escribir los datos
 * 		FALLO_ESCRITURA_EN_MEMORIA si hubo problemas al escribir los datos
 */
resultado_t escribir_en_memoria(direccion pid, direccion direccion,
	uint32_t bytes, char* buffer);

/*
 * @DESC: Le pide al kernel un tcb y guarda el tcb devuelto y su quantum y los parametros
 * @RETURNS:
 * 		OK si pudo conectarse y trajo bien los datos
 * 		FALLO_PEDIDO_DE_TCB si hubo problemas al traer los datos
 */
resultado_t pedir_tcb(tcb_t* tcb, int32_t* quantum);

/*
 * @DESC: Le avisa al kernel que termino de ejecutar el quantum siendo res el motivo de finalizacion
 * @RETURNS:
 * 		OK si pudo conectarse y mandar bien el mensaje
 * 		FALLO_INFORME_A_KERNEL si no pudo mandar bien el mensaje
 */
resultado_t informar_a_kernel_de_finalizacion(tcb_t tcb, resultado_t res);

/*
 * @DESC: Lee de memoria los primeros 4 bytes apuntados por pc y actualiza instruccion.
 * @RETURNS:
 */
resultado_t leer_proxima_instruccion(tcb_t* tcb, instruccion_t instruccion);

/*
 * @DESC: Lee de memoria un byte apuntado por pc y actualiza el registro.
 * @RETURNS:
 */
resultado_t leer_registro(tcb_t* tcb, char* registro);

/*
 * @DESC: Lee de memoria los primeros 4 bytes apuntados por pc y actualiza el numero.
 * @RETURNS:
 */
resultado_t leer_numero(tcb_t* tcb, int32_t* numero);

/*
 * @DESC: 	Le manda al kernel un mensaje para que pida por consola tantos bytes_a_leer,
 * 			siendo un ENTERO o CADENA segun identificador y
 * 			guardando el resultado en buffer
 * @RETURNS:
 * 		OK si pudo completar la operacion satisfactoriamente
 * 		FALLO_COMUNICACION si hubo algun problema de comunicacion
 */
resultado_t comunicar_entrada_estandar(tcb_t* tcb, uint32_t bytes_a_leer, uint32_t* bytes_leidos,
	char* buffer, idetificador_tipo_t identificador);

/*
 * @DESC: 	Le manda al kernel un mensaje para que imprima por consola tantos bytes_a_enviar,
 * 			siendo buffer la cadena.
 * @RETURNS:
 * 		OK si pudo completar la operacion satisfactoriamente
 * 		FALLO_COMUNICACION si hubo algun problema de comunicacion
 */
resultado_t comunicar_salida_estandar(tcb_t* tcb, uint32_t bytes_a_enviar,
	char* buffer);

/*
 * @DESC: 	Le manda al kernel un mensaje con el nuevo_tcb para que lo programe
 * @RETURNS:
 * 		OK si pudo completar la operacion satisfactoriamente
 * 		FALLO_COMUNICACION si hubo algun problema de comunicacion
 */
resultado_t comunicar_nuevo_tcb(tcb_t* nuevo_tcb);

/*
 * @DESC: 	Le manda al kernel un mensaje para que bloquee un hilo hasta que otro se ejecute
 * @RETURNS:
 * 		OK si pudo completar la operacion satisfactoriamente
 * 		FALLO_COMUNICACION si hubo algun problema de comunicacion
 */
resultado_t comunicar_join(uint32_t tid_llamador, uint32_t tid_esperador);

/*
 * @DESC: 	Le manda al kernel un mensaje para que bloquee un tcb hasta que se libere id_recurso
 * @RETURNS:
 * 		OK si pudo completar la operacion satisfactoriamente
 * 		FALLO_COMUNICACION si hubo algun problema de comunicacion
 */
resultado_t comunicar_bloquear(tcb_t* tcb, uint32_t id_recurso);

/*
 * @DESC: 	Le manda al kernel un mensaje para que libere un tcb que este esperando id_recurso
 * @RETURNS:
 * 		OK si pudo completar la operacion satisfactoriamente
 * 		FALLO_COMUNICACION si hubo algun problema de comunicacion
 */
resultado_t comunicar_despertar(tcb_t* tcb, uint32_t id_recurso);

// TODO eliminar (probablemente ya no sirva)
resultado_t pedir_tid_a_kernel(tcb_t tcb, direccion* nuevo_tid);

// TODO eliminar (ya no se hace falta)
//void pedir_al_kernel_tamanio_stack(uint32_t* tamanio_stack);

#endif /* MEMORIA_H_ */
