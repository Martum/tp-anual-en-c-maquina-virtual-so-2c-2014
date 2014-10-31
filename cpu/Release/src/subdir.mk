################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/configuraciones.c \
../src/instrucciones.c \
../src/main.c \
../src/operaciones-con-bits.c \
../src/sockets.c \
../src/tcb-funciones.c 

OBJS += \
./src/configuraciones.o \
./src/instrucciones.o \
./src/main.o \
./src/operaciones-con-bits.o \
./src/sockets.o \
./src/tcb-funciones.o 

C_DEPS += \
./src/configuraciones.d \
./src/instrucciones.d \
./src/main.d \
./src/operaciones-con-bits.d \
./src/sockets.d \
./src/tcb-funciones.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


