
#include <stdlib.h>
#include <stdio.h>

#include <commons/collections/list.h>

t_list* lista_conexiones_cpu;

void inicializar_lista_conexiones_cpu(){
	lista_conexiones_cpu = list_create();
}

void* escuchar_cpus(void* otro_ente)
{
	// TODO: SIMILAR A LA SUPERIOR PERO PARA ESCUCHAR CPUS

	return NULL;
}
