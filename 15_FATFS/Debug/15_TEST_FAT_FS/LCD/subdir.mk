################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../15_TEST_FAT_FS/LCD/lcd44780.c 

OBJS += \
./15_TEST_FAT_FS/LCD/lcd44780.o 

C_DEPS += \
./15_TEST_FAT_FS/LCD/lcd44780.d 


# Each subdirectory must supply rules for building sources it contributes
15_TEST_FAT_FS/LCD/%.o: ../15_TEST_FAT_FS/LCD/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


