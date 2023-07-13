################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nowy/d_led.c \
../nowy/main.c 

OBJS += \
./nowy/d_led.o \
./nowy/main.o 

C_DEPS += \
./nowy/d_led.d \
./nowy/main.d 


# Each subdirectory must supply rules for building sources it contributes
nowy/%.o: ../nowy/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


