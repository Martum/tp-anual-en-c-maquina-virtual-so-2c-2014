/*
 * funciones_streams.c
 *
 *  Created on: 17/11/2014
 *      Author: utnso
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* concat_string(char* a, char* b)
{
	char * aux = (char *) malloc(1+strlen(a)+strlen(b));
	strcpy(aux,a);
	strcat(aux,b);

	return aux;
}

char* substream(char* datos, uint16_t desp, uint32_t tam)
{
	int i;
	int j=0;
	char caracter;
	char* aux=malloc(tam);
	for(i=0; i<tam; i++)
	{
		caracter= *(datos+i+desp);
		*(aux+j)=caracter;

		j++;
	}
	return aux;
}
