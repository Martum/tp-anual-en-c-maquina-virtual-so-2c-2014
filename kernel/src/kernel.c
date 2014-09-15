/*
 ============================================================================
 Name        : kernel.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int jose = 5;

t_list* ready;
t_list* block;
t_list* exec;

int main(void) {
	printf("!!!%d!!!", jose); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
