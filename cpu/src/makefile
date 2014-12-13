-include ../../fuentes.mk

CC := gcc

ARCHIVOS := ansisop.o configuraciones.o instrucciones.o logs.o main.o operaciones-con-bits.o signals.o sockets.o tcb-funciones.o
NOMBREFINAL := cpu
LIBRERIAS := -lcommons -lhu4sockets -lansisop-panel

instalar: $(ARCHIVOS)
	$(CC) -o $(NOMBREFINAL) $(ARCHIVOS) -I$(LIBH4S) -I$(LIBCOMM) -I$(LIBAS) -L$(LIBH4SEXEC) -L$(LIBCOMMEXEC) -L$(LIBASEXEC) $(LIBRERIAS)

%.o:%.c
	$(CC) -c "$<" -I$(LIBH4S) -I$(LIBCOMM) -o "$@" $(LIBRERIAS)

desinstalar:
	rm -rf $(NOMBREFINAL) $(ARCHIVOS) 
