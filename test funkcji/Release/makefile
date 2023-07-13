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
testfunkcji.lss \

FLASH_IMAGE += \
testfunkcji.hex \

EEPROM_IMAGE += \
testfunkcji.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: testfunkcji.elf secondary-outputs

# Tool invocations
testfunkcji.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,testfunkcji.map -mmcu=atmega32 -o"testfunkcji.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

testfunkcji.lss: testfunkcji.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S testfunkcji.elf  >"testfunkcji.lss"
	@echo 'Finished building: $@'
	@echo ' '

testfunkcji.hex: testfunkcji.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex testfunkcji.elf  "testfunkcji.hex"
	@echo 'Finished building: $@'
	@echo ' '

testfunkcji.eep: testfunkcji.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex testfunkcji.elf  "testfunkcji.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: testfunkcji.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 testfunkcji.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) testfunkcji.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
