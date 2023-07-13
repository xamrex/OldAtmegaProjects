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
WinAmp.lss \

FLASH_IMAGE += \
WinAmp.hex \

EEPROM_IMAGE += \
WinAmp.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: WinAmp.elf secondary-outputs

# Tool invocations
WinAmp.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,WinAmp.map -mmcu=atmega32 -o"WinAmp.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

WinAmp.lss: WinAmp.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S WinAmp.elf  >"WinAmp.lss"
	@echo 'Finished building: $@'
	@echo ' '

WinAmp.hex: WinAmp.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex WinAmp.elf  "WinAmp.hex"
	@echo 'Finished building: $@'
	@echo ' '

WinAmp.eep: WinAmp.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex WinAmp.elf  "WinAmp.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: WinAmp.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 WinAmp.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) WinAmp.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
