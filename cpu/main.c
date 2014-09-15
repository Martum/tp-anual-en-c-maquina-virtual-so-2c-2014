#include <stdio.h>

int main(int argc, char** argv) {

	printf("Cargo el diccionario de instrucciones (normales y protegidas)\n");
	printf("Me conecto con la memoria MSP. Si falla la conexion aborto toda la ejecucion, informo por pantalla y loggeo\n");
	printf("Me conecto con el Kernel. Si falla la conexion aborto toda la ejecucion, informo por pantalla y loggeo\n");
	printf("Le pido al Kernel un TCB y me quedo esperando a que me lo de\n\n\n");
	printf("Me llego un TCB junto con su quantum\n\n");
	printf("Cargo en un TCBi (interno) el TCB que me pasaron")
	printf("Me fijo que el quantum no sea igual a 0, o que KM este activado\n");
	printf("Le mando a memoria un mensaje pidiendole 4 bytes a partir de la direccion PC\n");
	printf("Le pido al diccionario de instrucciones que me de la funcion que matchea con esos 4 bytes\n");
	printf("Ejecuto la funcion pasandole como parametro el TCBi\n");
	printf("La funcion modifica TCBi y devuelve un numero (res) que me indica como salio todo\n");
	printf("Le resto un punto al quantum");
	printf("Si el quantum no es igual a 0, res no indique se terminaron las instrucciones, repito los pasos\n\n");
	printf("El quantum fue igual a 0, o termino el proceso\n");
	printf("Le mando un mensaje al Kernel pasandole el TCBi y la indicacion de porque termine de ejecutar el TCB\n");
	printf("Me quedo esperando a que el Kernel me mande otro TCB\n");

	return 0;
}
