################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include LCD/subdir.mk
-include IR_DECODE/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Gra-Artura.lss \

FLASH_IMAGE += \
Gra-Artura.hex \

EEPROM_IMAGE += \
Gra-Artura.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: Gra-Artura.elf secondary-outputs

# Tool invocations
Gra-Artura.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Gra-Artura.map -mmcu=atmega32 -o"Gra-Artura.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Gra-Artura.lss: Gra-Artura.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Gra-Artura.elf  >"Gra-Artura.lss"
	@echo 'Finished building: $@'
	@echo ' '

Gra-Artura.hex: Gra-Artura.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex Gra-Artura.elf  "Gra-Artura.hex"
	@echo 'Finished building: $@'
	@echo ' '

Gra-Artura.eep: Gra-Artura.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex Gra-Artura.elf  "Gra-Artura.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Gra-Artura.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 Gra-Artura.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) Gra-Artura.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
