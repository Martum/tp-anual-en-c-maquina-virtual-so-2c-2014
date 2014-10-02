/*
 * consola_msp.c
 *
 *  Created on: 17/09/2014
 *      Author: utnso
 */
#include <stdlib.h>
#include "estructuras.h"
#include <stdio.h>

#include "consola_msp.h"
#include "configuraciones.h"
#include "algoritmos_sustitucion.h"



int main(void){
	cargar_configuraciones();
	inicializar_lista_procesos();
	inicializar_lista_marcos();
	inicializar_indice_paginas();
	inicializar_memoria_fisica_total();
	return 0;
}

// PRUEBAS

/*
 * prueba que anda el algoritmo lru
 *
	t_list* lista_paginas = list_create();

	pagina_t* pagina1 = malloc(sizeof(pagina_t));
	pagina1->id = 0;
	pagina1->marco = 2;
	pagina1->ultima_vez_usada = "02:01:20:1000";

	pagina_t* pagina2 = malloc(sizeof(pagina_t));
	pagina2->id = 12;
	pagina2->marco = 5;
	pagina2->ultima_vez_usada = "02:01:20:2000";

	pagina_t* pagina3 = malloc(sizeof(pagina_t));
	pagina3->id = 2;
	pagina3->marco = 4;
	pagina3->ultima_vez_usada = "02:01:14:1000";

	list_add(lista_paginas,pagina1);
	list_add(lista_paginas,pagina2);
	list_add(lista_paginas,pagina3);

	// busco el que fue usado menos recientemente
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

	list_sort(lista_paginas, (void*) _comparo_ultimo_uso);

	pagina_t* pag_menos_usada = malloc(sizeof(pagina_t));
	pag_menos_usada = (pagina_t*)list_get(lista_paginas,0);
	uint32_t id_marco_elegido = pag_menos_usada->marco;

	printf("%d",id_marco_elegido);
	free(pag_menos_usada);
*/
	/*
	 * pruebo si anda archivo de configuracion
	 *
	printf("puerto: %d \n",puerto());
	printf("cantidad_memoria: %d \n", cantidad_memoria());
	printf("cantidad_swap: %d \n",cantidad_swap());
	printf("algoritmo_sustitucion: %s",algoritmo_sustitucion_de_paginas());
	*/
