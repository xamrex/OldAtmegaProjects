################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include PetitFS/subdir.mk
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
avrcard.lss \

FLASH_IMAGE += \
avrcard.hex \

EEPROM_IMAGE += \
avrcard.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: avrcard.elf secondary-outputs

# Tool invocations
avrcard.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,avrcard.map -mmcu=atmega32 -o"avrcard.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

avrcard.lss: avrcard.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S avrcard.elf  >"avrcard.lss"
	@echo 'Finished building: $@'
	@echo ' '

avrcard.hex: avrcard.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex avrcard.elf  "avrcard.hex"
	@echo 'Finished building: $@'
	@echo ' '

avrcard.eep: avrcard.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex avrcard.elf  "avrcard.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: avrcard.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 avrcard.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) avrcard.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
