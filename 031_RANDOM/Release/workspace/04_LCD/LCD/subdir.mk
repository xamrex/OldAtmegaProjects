################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../workspace/04_LCD/LCD/lcd44780.c 

OBJS += \
./workspace/04_LCD/LCD/lcd44780.o 

C_DEPS += \
./workspace/04_LCD/LCD/lcd44780.d 


# Each subdirectory must supply rules for building sources it contributes
workspace/04_LCD/LCD/%.o: ../workspace/04_LCD/LCD/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


