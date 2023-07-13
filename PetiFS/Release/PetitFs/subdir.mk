################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PetitFs/mmc.c \
../PetitFs/pff.c 

OBJS += \
./PetitFs/mmc.o \
./PetitFs/pff.o 

C_DEPS += \
./PetitFs/mmc.d \
./PetitFs/pff.d 


# Each subdirectory must supply rules for building sources it contributes
PetitFs/%.o: ../PetitFs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=11059200UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


