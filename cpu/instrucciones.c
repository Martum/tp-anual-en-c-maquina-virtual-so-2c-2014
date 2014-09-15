#include <stdio.h>
#include <commons/collections/dictionary.h>
#include "estructuras.h"


void cargar_diccionario_de_instrucciones(t_dictionary* dic) {
	dictionary_put(dic, "LOAD", load);
}

int load(tcb_t tcb) {
	/*
	// Pido a memoria 1 byte del registro a partir de tcb->pc y lo guardo en registro
	char* registro = malloc(sizeof(char));
	registro = leer_memoria(tcb->pc, sizeof(char));
	tcb.pc = tcb.pc + 1;

	// Pido a memoria 4 byte del numero a partir de tcb->pc y lo guardo en numero
	int32_t numero = leer_memoria(tcb->pc, sizeof(int32_t));
	tcb.pc = tcb.pc + 4;

	switch (registro) {
	case 'a':
		tcb.a = numero;
		break;
	case 'b':
		tcb.b = numero;
		break;
	case 'c':
		tcb.c = numero;
		break;
	case 'd':
		tcb.d = numero;
		break;
	case 'e':
		tcb.e = numero;
		break;
	default:
		return -1;
	}*/
	return 0;
}

