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
s65v2.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: s65v2.elf secondary-outputs

# Tool invocations
s65v2.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,s65v2.map -mmcu=atmega32 -o"s65v2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

s65v2.lss: s65v2.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S s65v2.elf  >"s65v2.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: s65v2.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 s65v2.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) s65v2.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
