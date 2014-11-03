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
 * 	@DESC: Devuelve la funcion asosiada a la instruccion
 * 	@RETURN: una funcion
 */
void obtener_funcion_segun_instruccion(resultado_t (*funcion)(tcb_t*), instruccion_t instruccion);

#endif /* INSTRUCCIONES_H_ */
