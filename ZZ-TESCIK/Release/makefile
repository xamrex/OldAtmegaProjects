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
ZZ-TESCIK.lss \

FLASH_IMAGE += \
ZZ-TESCIK.hex \

EEPROM_IMAGE += \
ZZ-TESCIK.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: ZZ-TESCIK.elf secondary-outputs

# Tool invocations
ZZ-TESCIK.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,ZZ-TESCIK.map -mmcu=attiny2313 -o"ZZ-TESCIK.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ZZ-TESCIK.lss: ZZ-TESCIK.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S ZZ-TESCIK.elf  >"ZZ-TESCIK.lss"
	@echo 'Finished building: $@'
	@echo ' '

ZZ-TESCIK.hex: ZZ-TESCIK.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex ZZ-TESCIK.elf  "ZZ-TESCIK.hex"
	@echo 'Finished building: $@'
	@echo ' '

ZZ-TESCIK.eep: ZZ-TESCIK.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex ZZ-TESCIK.elf  "ZZ-TESCIK.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: ZZ-TESCIK.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=attiny2313 ZZ-TESCIK.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) ZZ-TESCIK.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
