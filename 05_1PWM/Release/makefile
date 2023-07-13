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
05_1PWM.lss \

FLASH_IMAGE += \
05_1PWM.hex \

EEPROM_IMAGE += \
05_1PWM.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 05_1PWM.elf secondary-outputs

# Tool invocations
05_1PWM.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,05_1PWM.map -mmcu=atmega32 -o"05_1PWM.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

05_1PWM.lss: 05_1PWM.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 05_1PWM.elf  >"05_1PWM.lss"
	@echo 'Finished building: $@'
	@echo ' '

05_1PWM.hex: 05_1PWM.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex 05_1PWM.elf  "05_1PWM.hex"
	@echo 'Finished building: $@'
	@echo ' '

05_1PWM.eep: 05_1PWM.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 05_1PWM.elf  "05_1PWM.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 05_1PWM.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 05_1PWM.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) 05_1PWM.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
