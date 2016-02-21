################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(PLATINUM)/platinum.c" \

C_SRCS += \
$(PLATINUM)/platinum.c \

OBJS += \
./project/platinum.o \

C_DEPS += \
./project/platinum.d \

OBJS_QUOTED += \
"./project/platinum.o" \

C_DEPS_QUOTED += \
"./project/platinum.d" \

OBJS_OS_FORMAT += \
./project/platinum.o \


# Each subdirectory must supply rules for building sources it contributes
project/platinum.o: $(PLATINUM)/platinum.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"project/platinum.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"project/platinum.o"
	@echo 'Finished building: $<'
	@echo ' '


