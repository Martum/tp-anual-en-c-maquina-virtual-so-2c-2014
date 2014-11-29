# Makefile template for shared library
 
CC = gcc # C compiler
CFLAGS = -fPIC -Wall -Wextra -O2 -g # C flags
LDFLAGS = -shared # linking flags
RM = rm -f # rm command
TARGET_LIB = libhu4sockets.so # target lib
 
SRCS = mensajes.c resultados.c sockets.c tcb.c # source files
OBJS = $(SRCS:.c=.o)
 
 .PHONY: instalar
instalar: ${TARGET_LIB}
 
$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^
 
$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@
 
include $(SRCS:.c=.d)
 
 .PHONY: desinstalar
desinstalar:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d) 
