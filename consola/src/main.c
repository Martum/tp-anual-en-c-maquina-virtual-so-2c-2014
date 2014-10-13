/*#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	FILE *entrada, *salida;
	char buffer[1000];
	int leidos;

	entrada = fopen("out.bc", "rb");
	salida = fopen("out2.bc", "rb+");

	if (entrada == NULL ) { // Error de apertura de entrada
		printf("\nError de apertura de entrada. \n\n");
	} else if (salida == NULL) {// Error de apertura de salida
		printf("\nError de apertura de salida. \n\n");
	} else { // Abrio los archivos
		printf("\nEl contenido del archivo de prueba es \n\n");
		while (feof(entrada) == 0) {
			leidos = fread (buffer, 1, 1000, entrada);
			printf("%d", leidos);
			leidos = fwrite (buffer, 1, leidos, salida);
			printf("%d", leidos);

		}
	}
	fclose(entrada);
	fclose(salida);
	return 0;
}*/
