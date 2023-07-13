################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include PetitFs/subdir.mk
-include LCD/subdir.mk
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
PetiFS.lss \

FLASH_IMAGE += \
PetiFS.hex \

EEPROM_IMAGE += \
PetiFS.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: PetiFS.elf secondary-outputs

# Tool invocations
PetiFS.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,PetiFS.map -mmcu=atmega32 -o"PetiFS.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

PetiFS.lss: PetiFS.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S PetiFS.elf  >"PetiFS.lss"
	@echo 'Finished building: $@'
	@echo ' '

PetiFS.hex: PetiFS.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex PetiFS.elf  "PetiFS.hex"
	@echo 'Finished building: $@'
	@echo ' '

PetiFS.eep: PetiFS.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex PetiFS.elf  "PetiFS.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: PetiFS.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 PetiFS.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) PetiFS.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
