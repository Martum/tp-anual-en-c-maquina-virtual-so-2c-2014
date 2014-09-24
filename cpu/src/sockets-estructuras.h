/*
 * sockets-estructuras.h
 *
 *  Created on: 23/09/2014
 *      Author: utnso
 */

#ifndef SOCKETS_ESTRUCTURAS_H_
#define SOCKETS_ESTRUCTURAS_H_

typedef enum {MANDA_UN_TCB} flags_t;

typedef struct mensaje {
	flags_t flag;
} mensaje_aviso_t;

typedef struct mensaje_tcb {
	uint32_t tcb; // todo cambiar por tcb
	uint32_t quantum;
} mensaje_tcb_t;

#endif /* SOCKETS_ESTRUCTURAS_H_ */
