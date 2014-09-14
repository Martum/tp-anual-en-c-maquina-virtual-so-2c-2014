/*
 * marco.h
 *
 *  Created on: 14/09/2014
 *      Author: utnso
 */

#ifndef MARCO_H_
#define MARCO_H_

#include <stdbool.h>
#include "direccion.h"

typedef struct marco{
	bool ocupado;
	direccion base;
}marco_t;

#endif /* MARCO_H_ */
