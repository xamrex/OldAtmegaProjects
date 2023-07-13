################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include 15_TEST_FAT_FS/subdir.mk
-include 15_TEST_FAT_FS/Release/subdir.mk
-include 15_TEST_FAT_FS/Release/LCD/subdir.mk
-include 15_TEST_FAT_FS/Release/FatFS/subdir.mk
-include 15_TEST_FAT_FS/LCD/subdir.mk
-include 15_TEST_FAT_FS/FatFS/subdir.mk
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
15_FATFS.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: 15_FATFS.elf secondary-outputs

# Tool invocations
15_FATFS.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,15_FATFS.map -mmcu=atmega16 -o"15_FATFS.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

15_FATFS.lss: 15_FATFS.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 15_FATFS.elf  >"15_FATFS.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 15_FATFS.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega16 15_FATFS.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) 15_FATFS.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
