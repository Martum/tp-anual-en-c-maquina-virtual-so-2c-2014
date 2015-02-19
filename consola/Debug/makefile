################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: consola

dependents:
	-cd /home/santiago/workspace/c/tp-2014/libs/Debug && $(MAKE) all

# Tool invocations
consola: $(OBJS) $(USER_OBJS) /home/santiago/workspace/c/tp-2014/libs/Debug/libhu4sockets.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/santiago/workspace/c/tp-2014/libs/Debug" -o "consola" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) consola
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/santiago/workspace/c/tp-2014/libs/Debug/libhu4sockets.so:

-include ../makefile.targets
