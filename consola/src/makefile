-include ../../fuentes.mk

CC := gcc

instalar: configuraciones.o consola.o
	$(CC) -o consola consola.o configuraciones.o -I$(LIBS) -I$(LIBCOMM) -L$(LIBH4S) -L$(LIBCOMM) -lcommons -lpthread -lhu4sockets

%.o:%.c
	$(CC) -c "$<" -I$(LIBS) -I$(LIBCOMM) -o "$@" -lcommons -lpthread -lhu4sockets

desinstalar:
	rm -rf consola configuraciones.o consola.o
