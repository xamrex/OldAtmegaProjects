################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RS/rs232.c 

OBJS += \
./RS/rs232.o 

C_DEPS += \
./RS/rs232.d 


# Each subdirectory must supply rules for building sources it contributes
RS/%.o: ../RS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


