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
FATFS.lss \

FLASH_IMAGE += \
FATFS.hex \

EEPROM_IMAGE += \
FATFS.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: FATFS.elf secondary-outputs

# Tool invocations
FATFS.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,FATFS.map -mmcu=atmega32 -o"FATFS.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

FATFS.lss: FATFS.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S FATFS.elf  >"FATFS.lss"
	@echo 'Finished building: $@'
	@echo ' '

FATFS.hex: FATFS.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex FATFS.elf  "FATFS.hex"
	@echo 'Finished building: $@'
	@echo ' '

FATFS.eep: FATFS.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex FATFS.elf  "FATFS.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: FATFS.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 FATFS.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) FATFS.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
