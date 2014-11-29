-include ../../fuentes.mk

CC := gcc

ARCHIVOS := configuraciones.o consola.o
NOMBREFINAL := consola

instalar: $(ARCHIVOS)
	$(CC) -o $(NOMBREFINAL) $(ARCHIVOS) -I$(LIBH4S) -I$(LIBCOMM) -L$(LIBH4SEXEC) -L$(LIBCOMMEXEC) -lcommons -lpthread -lhu4sockets

%.o:%.c
	$(CC) -c "$<" -I$(LIBH4S) -I$(LIBCOMM) -o "$@" -lcommons -lpthread -lhu4sockets

desinstalar:
	rm -rf $(NOMBREFINAL) $(ARCHIVOS)
