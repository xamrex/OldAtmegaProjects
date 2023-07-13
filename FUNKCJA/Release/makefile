################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
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
FUNKCJA.lss \

FLASH_IMAGE += \
FUNKCJA.hex \

EEPROM_IMAGE += \
FUNKCJA.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: FUNKCJA.elf secondary-outputs

# Tool invocations
FUNKCJA.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,FUNKCJA.map -mmcu=atmega32 -o"FUNKCJA.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

FUNKCJA.lss: FUNKCJA.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S FUNKCJA.elf  >"FUNKCJA.lss"
	@echo 'Finished building: $@'
	@echo ' '

FUNKCJA.hex: FUNKCJA.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex FUNKCJA.elf  "FUNKCJA.hex"
	@echo 'Finished building: $@'
	@echo ' '

FUNKCJA.eep: FUNKCJA.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex FUNKCJA.elf  "FUNKCJA.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: FUNKCJA.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 FUNKCJA.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) FUNKCJA.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
