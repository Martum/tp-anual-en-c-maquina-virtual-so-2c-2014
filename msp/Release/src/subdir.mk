################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/algoritmos_sustitucion.c \
../src/conexiones.c \
../src/configuraciones.c \
../src/consola_msp.c \
../src/direcciones.c \
../src/interfaz.c \
../src/main.c \
../src/marco.c \
../src/pagina.c \
../src/proceso_msp.c \
../src/segmento.c \
../src/swapping.c 

OBJS += \
./src/algoritmos_sustitucion.o \
./src/conexiones.o \
./src/configuraciones.o \
./src/consola_msp.o \
./src/direcciones.o \
./src/interfaz.o \
./src/main.o \
./src/marco.o \
./src/pagina.o \
./src/proceso_msp.o \
./src/segmento.o \
./src/swapping.o 

C_DEPS += \
./src/algoritmos_sustitucion.d \
./src/conexiones.d \
./src/configuraciones.d \
./src/consola_msp.d \
./src/direcciones.d \
./src/interfaz.d \
./src/main.d \
./src/marco.d \
./src/pagina.d \
./src/proceso_msp.d \
./src/segmento.d \
./src/swapping.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


