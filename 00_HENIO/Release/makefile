################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include I2C_SOFT/subdir.mk
-include 1Wire/subdir.mk
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
00_HENIO.lss \

FLASH_IMAGE += \
00_HENIO.hex \

EEPROM_IMAGE += \
00_HENIO.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 00_HENIO.elf secondary-outputs

# Tool invocations
00_HENIO.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,00_HENIO.map -mmcu=atmega8 -o"00_HENIO.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

00_HENIO.lss: 00_HENIO.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 00_HENIO.elf  >"00_HENIO.lss"
	@echo 'Finished building: $@'
	@echo ' '

00_HENIO.hex: 00_HENIO.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex 00_HENIO.elf  "00_HENIO.hex"
	@echo 'Finished building: $@'
	@echo ' '

00_HENIO.eep: 00_HENIO.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 00_HENIO.elf  "00_HENIO.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 00_HENIO.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega8 00_HENIO.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) 00_HENIO.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
