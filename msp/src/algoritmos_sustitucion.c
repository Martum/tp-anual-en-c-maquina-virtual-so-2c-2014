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
#include "semaforos.h"

uint32_t puntero_clock;
void setear_puntero_clock(){
	puntero_clock = 0;
}

uint32_t algoritmo_clock(uint16_t * id_pagina_a_swappear){
	uint32_t retorno=-1;
	bool encontro = false;

	// filtro a las paginas segun las que tienen marco
	bool _paginas_con_marco(pagina_t* pagina){
		return pagina->tiene_marco;
	}

//	lock_lista_indice_paginas();
	t_list* paginas_con_marco =	list_filter(get_indice_paginas(),(void*) _paginas_con_marco);
//	unlock_lista_indice_paginas();

	// itero las paginas
	pagina_t* pag = malloc(sizeof(pagina_t));

	while(puntero_clock<list_size(paginas_con_marco) && (retorno == -1)){
		pag = list_get(paginas_con_marco,puntero_clock);
		if(pag->bit_referencia == 1){
			pag->bit_referencia = 0;
			list_replace(paginas_con_marco,puntero_clock,pag);
		}else{
			retorno = pag->marco;
			encontro = true;
		}
		puntero_clock++;
	}

	if(!encontro){
		puntero_clock = 0;

		while(puntero_clock<list_size(paginas_con_marco) && (retorno == -1)){
			pag = list_get(paginas_con_marco,puntero_clock);
			if(pag->bit_referencia == 1){
				pag->bit_referencia = 0;
				list_replace(paginas_con_marco,puntero_clock,pag);
			}else{
				retorno = pag->marco;
			}
			puntero_clock++;
		}
	}

	//TODO Checkear que esto no se borra con el free
	*id_pagina_a_swappear=pag->id;

	free(paginas_con_marco);
	free(pag);
	return retorno;
}

uint32_t algoritmo_lru(uint16_t * id_pagina_a_swappear){
	// filtro segun paginas que tienen marco
	bool _paginas_con_marco(pagina_t* pagina){
		return pagina->tiene_marco;
	}

	//lock_lista_indice_paginas();
	t_list* paginas_con_marco =	list_filter(get_indice_paginas(),(void*) _paginas_con_marco);
	//unlock_lista_indice_paginas();

	// saco el ultimo elemento (el menos usado recientemente)
	pagina_t* pag = list_get(paginas_con_marco,list_size(paginas_con_marco)-1);
	uint32_t resultado = pag->marco;


	//TODO Checkear que esto no se borra con el free
	*id_pagina_a_swappear=pag->id;

	free(pag);
	free(paginas_con_marco);
	return resultado;
}

void ubico_al_principio(pagina_t* pag){
	//lock_lista_indice_paginas();
	list_remove(get_indice_paginas(),pag->id_en_indice);
	list_add_in_index(get_indice_paginas(),0,pag);
	//unlock_lista_indice_paginas();
	pag->id_en_indice = 0;
}

void set_bit_referencia(pagina_t* pag){
	pag->bit_referencia = 1;
}


