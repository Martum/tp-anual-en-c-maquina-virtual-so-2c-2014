/*
 * algoritmos_sustitucion.c
 *
 *  Created on: 29/09/2014
 *      Author: utnso
 */

#include <stdbool.h>
#include <stdlib.h>

#include <commons/collections/list.h>
#include <commons/string.h>

#include "marco.h"
#include "estructuras.h"
#include "configuraciones.h"


uint32_t clock(){
	uint32_t retorno = -1;

	// filtro a las paginas segun las que tienen marco
	bool _paginas_con_marco(pagina_t* pagina){
		return pagina->tiene_marco;
	}
	t_list* paginas_con_marco =	list_filter(get_indice_paginas(),(void*) _paginas_con_marco);

	// itero las paginas
	pagina_t* pag = malloc(sizeof(pagina_t));
	int i = 0;
	while(i<list_size(paginas_con_marco) && (retorno == -1)){
		pag = list_get(paginas_con_marco,i);
		if(pag->bit_referencia == 1){
			pag->bit_referencia = 0;
			list_replace(paginas_con_marco,i,pag);
		}else{
			retorno = pag->marco;
		}
		i++;
	}
	if(retorno == -1){
		pag = list_get(paginas_con_marco,0);
		retorno = pag->marco;
	}
	free(paginas_con_marco);
	free(pag);
	return retorno;
}

uint32_t lru(){
	// filtro segun paginas que tienen marco
	bool _paginas_con_marco(pagina_t* pagina){
		return pagina->tiene_marco;
	}
	t_list* paginas_con_marco =	list_filter(get_indice_paginas(),(void*) _paginas_con_marco);
	// saco el ultimo elemento (el menos usado recientemente)
	pagina_t* pag = list_get(paginas_con_marco,list_size(paginas_con_marco)-1);
	uint32_t resultado = pag->marco;
	free(pag);
	free(paginas_con_marco);
	return resultado;
}
/*
void ubico_al_principio(pagina_t* pag){
	list_remove(get_indice_paginas(),1);
	list_add_in_index(get_indice_paginas(),0,pagina3);
}
// VIEJO LRU

uint32_t lru(){
	// filtro a las paginas segun las que tienen marco
	bool _paginas_con_marco(pagina_t* pagina){
		return pagina->tiene_marco;
	}
	t_list* paginas_con_marco =	list_filter(get_indice_paginas(),(void*) _paginas_con_marco);

	// funcion que compara segun horas, minutos, segundos y milisegundos de ultimo acceso
	bool _comparo_horas(char* hora1, char* hora2){
		int h1 = atoi(string_substring_until(hora1, 2));
		int h2 = atoi(string_substring_until(hora2, 2));

		int m1 = atoi(string_substring(hora1, 3, 2));
		int m2 = atoi(string_substring(hora2, 3, 2));

		int s1 = atoi(string_substring(hora1, 6, 2));
		int s2 = atoi(string_substring(hora2, 6, 2));

		int ms1 = atoi(string_substring(hora1, 9, 4));
		int ms2 = atoi(string_substring(hora2, 9, 4));

		bool hora2_es_mayor = false;
		if(h1 < h2){
			hora2_es_mayor = true;
		}
		if(h1 == h2){
			if(m1 < m2){
				hora2_es_mayor = true;
			}
			if(m1 == m2){
				if(s1 < s2){
					hora2_es_mayor = true;
				}
				if(s1 == s2){
					if(ms1 < ms2){
						hora2_es_mayor = true;
					}
				}
			}
		}
		return hora2_es_mayor;
	}

	bool _comparo_ultimo_uso(pagina_t* p1, pagina_t* p2){
		return _comparo_horas(p1->ultima_vez_usada, p2->ultima_vez_usada);
	}
	// ordeno la lista segun ultimo uso
	list_sort(paginas_con_marco, (void*) _comparo_ultimo_uso);

	pagina_t* pag_menos_usada = malloc(sizeof(pagina_t));
	// saco la primera pagina de la lista
	pag_menos_usada = (pagina_t*)list_get(paginas_con_marco,0);
	// retorno el id de su marco
	uint32_t id_marco_elegido = pag_menos_usada->marco;
	free(pag_menos_usada);
	free(paginas_con_marco);
	return id_marco_elegido;
}
*/
