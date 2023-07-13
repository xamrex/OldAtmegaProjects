################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ata.c \
../cc932_avr.c \
../cfc.c \
../ff.c \
../main.c \
../mmc.c \
../mmcbb.c \
../uart.c 

S_UPPER_SRCS += \
../xitoa.S 

OBJS += \
./ata.o \
./cc932_avr.o \
./cfc.o \
./ff.o \
./main.o \
./mmc.o \
./mmcbb.o \
./uart.o \
./xitoa.o 

C_DEPS += \
./ata.d \
./cc932_avr.d \
./cfc.d \
./ff.d \
./main.d \
./mmc.d \
./mmcbb.d \
./uart.d 

S_UPPER_DEPS += \
./xitoa.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=11059200UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -mmcu=atmega32 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


