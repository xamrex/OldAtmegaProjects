################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include LCD/subdir.mk
-include I2C_SOFT/subdir.mk
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
08_I2C_SOFT_ORYGINAL.lss \

FLASH_IMAGE += \
08_I2C_SOFT_ORYGINAL.hex \

EEPROM_IMAGE += \
08_I2C_SOFT_ORYGINAL.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 08_I2C_SOFT_ORYGINAL.elf secondary-outputs

# Tool invocations
08_I2C_SOFT_ORYGINAL.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,08_I2C_SOFT_ORYGINAL.map -mmcu=atmega32 -o"08_I2C_SOFT_ORYGINAL.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

08_I2C_SOFT_ORYGINAL.lss: 08_I2C_SOFT_ORYGINAL.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 08_I2C_SOFT_ORYGINAL.elf  >"08_I2C_SOFT_ORYGINAL.lss"
	@echo 'Finished building: $@'
	@echo ' '

08_I2C_SOFT_ORYGINAL.hex: 08_I2C_SOFT_ORYGINAL.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex 08_I2C_SOFT_ORYGINAL.elf  "08_I2C_SOFT_ORYGINAL.hex"
	@echo 'Finished building: $@'
	@echo ' '

08_I2C_SOFT_ORYGINAL.eep: 08_I2C_SOFT_ORYGINAL.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 08_I2C_SOFT_ORYGINAL.elf  "08_I2C_SOFT_ORYGINAL.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 08_I2C_SOFT_ORYGINAL.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 08_I2C_SOFT_ORYGINAL.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) 08_I2C_SOFT_ORYGINAL.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
