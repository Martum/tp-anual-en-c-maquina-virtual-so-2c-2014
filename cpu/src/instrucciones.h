/*
 * operaciones-con-bits.h
 *
 *  Created on: 17/10/2014
 *      Author: utnso
 */

#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

#include <commons/collections/dictionary.h>

#include "definiciones.h"
#include "operaciones-con-bits.h"
#include "sockets.h"
#include "tcb-funciones.h"

/*
 * 	@DESC: Crea el diccionario de instrucciones y le carga todas las instrucciones
 * 	@RETURN:
 */
void inicializar_dic_de_instrucciones();

/*
 * 	@DESC: Destruye el diccionario de instrucciones
 * 	@RETURN:
 */
void liberar_dic_de_instrucciones();

/*
 * 	@DESC: Carga todas las instrucciones en dic
 * 	@RETURN:
 */
void cargar_diccionario_de_instrucciones(t_dictionary* dic);

/*
 * 	@DESC: Lee la siguiente instruccion y la decodifica
 * 	@RETURN:
 */
void leer_siguiente_instruccion(tcb_t* tcb);

/*
 * 	@DESC: Ejecuta la instruccion
 * 	@RETURN:
 * 		OK si todo salio bien
 *		ERROR_EN_EJECUCION si algo fallo en la instruccion o no habia instruccion a ejecutar
 */
resultado_t ejecutar_instruccion(tcb_t* tcb);

#endif /* INSTRUCCIONES_H_ */
