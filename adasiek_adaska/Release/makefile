################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
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
adasiek.lss \

FLASH_IMAGE += \
adasiek.hex \

EEPROM_IMAGE += \
adasiek.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: adasiek.elf secondary-outputs

# Tool invocations
adasiek.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,adasiek.map -mmcu=atmega32 -o"adasiek.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

adasiek.lss: adasiek.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S adasiek.elf  >"adasiek.lss"
	@echo 'Finished building: $@'
	@echo ' '

adasiek.hex: adasiek.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex adasiek.elf  "adasiek.hex"
	@echo 'Finished building: $@'
	@echo ' '

adasiek.eep: adasiek.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex adasiek.elf  "adasiek.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: adasiek.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 adasiek.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) adasiek.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
