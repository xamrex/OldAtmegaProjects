################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include RS/subdir.mk
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
asd.lss \

FLASH_IMAGE += \
asd.hex \

EEPROM_IMAGE += \
asd.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: asd.elf secondary-outputs

# Tool invocations
asd.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,asd.map -mmcu=atmega32 -o"asd.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

asd.lss: asd.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S asd.elf  >"asd.lss"
	@echo 'Finished building: $@'
	@echo ' '

asd.hex: asd.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex asd.elf  "asd.hex"
	@echo 'Finished building: $@'
	@echo ' '

asd.eep: asd.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex asd.elf  "asd.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: asd.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 asd.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) asd.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
