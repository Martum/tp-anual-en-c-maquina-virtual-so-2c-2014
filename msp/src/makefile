-include ../../fuentes.mk

CC := gcc

ARCHIVOS := algoritmos_sustitucion.o conexiones.o configuraciones.o consola_msp.o direcciones.o estructuras.o funciones_streams.o interfaz.o logs.o main.o marco.o pagina.o proceso_msp.o segmento.o semaforos.o swapping.o 
NOMBREFINAL := msp
LIBRERIAS := -lcommons -lhu4sockets -lpthread

instalar: $(ARCHIVOS)
	$(CC) -o $(NOMBREFINAL) $(ARCHIVOS) -I$(LIBH4S) -I$(LIBCOMM) -I$(LIBAS) -L$(LIBH4SEXEC) -L$(LIBCOMMEXEC) -L$(LIBASEXEC) $(LIBRERIAS)

%.o:%.c
	$(CC) -c "$<" -I$(LIBH4S) -I$(LIBCOMM) -o "$@" $(LIBRERIAS)

desinstalar:
	rm -rf $(NOMBREFINAL) $(ARCHIVOS) 
