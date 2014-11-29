/*
 * ansisop.h
 *
 *  Created on: 25/11/2014
 *      Author: utnso
 */

#ifndef ANSISOP_H_
#define ANSISOP_H_

#include <stdint.h>
#include <stdlib.h>

#include <ansisop-panel/panel/panel.h>
#include <ansisop-panel/panel/cpu.h>

#include "commons/collections/list.h"
#include "commons/string.h"

#include "hu4sockets/tcb.h"

#include "definiciones.h"

void empezar_ansisop();
void ansisop_comienzo_tcb(tcb_t tcb, int32_t quantum);
void ansisop_fin_tcb();
void ansisop_ejecucion_instruccion1(instruccion_t instruccion);
void ansisop_ejecucion_instruccion2(instruccion_t instruccion, int32_t param_numero, char param_registro);
void ansisop_ejecucion_instruccion3(instruccion_t instruccion, int32_t param_numero);
void ansisop_ejecucion_instruccion4(instruccion_t instruccion, int32_t param_numero, char param_registro1, char param_registro2);
void ansisop_ejecucion_instruccion5(instruccion_t instruccion, char param_registro);
void ansisop_ejecucion_instruccion6(instruccion_t instruccion, char param_registro1, char param_registro2);
void ansisop_cambio_registro_a(int32_t nuevo_valor);
void ansisop_cambio_registro_b(int32_t nuevo_valor);
void ansisop_cambio_registro_c(int32_t nuevo_valor);
void ansisop_cambio_registro_d(int32_t nuevo_valor);
void ansisop_cambio_registro_e(int32_t nuevo_valor);
void ansisop_cambio_registro_m(direccion nuevo_valor);
void ansisop_cambio_registro_pc(direccion nuevo_valor);
void ansisop_cambio_registro_x(direccion nuevo_valor);
void ansisop_cambio_registro_cursor(direccion nuevo_valor);
void ansisop_cambio_registro_i(uint32_t nuevo_valor);
void ansisop_cambio_registro_k(bool nuevo_valor);

#endif /* ANSISOP_H_ */
