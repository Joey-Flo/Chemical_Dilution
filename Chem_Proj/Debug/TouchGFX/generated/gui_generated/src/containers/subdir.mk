################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX/generated/gui_generated/src/containers/PumpDropDownListBase.cpp \
../TouchGFX/generated/gui_generated/src/containers/PumpOptionBase.cpp \
../TouchGFX/generated/gui_generated/src/containers/PumpSetupWidgetBase.cpp 

OBJS += \
./TouchGFX/generated/gui_generated/src/containers/PumpDropDownListBase.o \
./TouchGFX/generated/gui_generated/src/containers/PumpOptionBase.o \
./TouchGFX/generated/gui_generated/src/containers/PumpSetupWidgetBase.o 

CPP_DEPS += \
./TouchGFX/generated/gui_generated/src/containers/PumpDropDownListBase.d \
./TouchGFX/generated/gui_generated/src/containers/PumpOptionBase.d \
./TouchGFX/generated/gui_generated/src/containers/PumpSetupWidgetBase.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX/generated/gui_generated/src/containers/%.o TouchGFX/generated/gui_generated/src/containers/%.su TouchGFX/generated/gui_generated/src/containers/%.cyclo: ../TouchGFX/generated/gui_generated/src/containers/%.cpp TouchGFX/generated/gui_generated/src/containers/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TouchGFX-2f-generated-2f-gui_generated-2f-src-2f-containers

clean-TouchGFX-2f-generated-2f-gui_generated-2f-src-2f-containers:
	-$(RM) ./TouchGFX/generated/gui_generated/src/containers/PumpDropDownListBase.cyclo ./TouchGFX/generated/gui_generated/src/containers/PumpDropDownListBase.d ./TouchGFX/generated/gui_generated/src/containers/PumpDropDownListBase.o ./TouchGFX/generated/gui_generated/src/containers/PumpDropDownListBase.su ./TouchGFX/generated/gui_generated/src/containers/PumpOptionBase.cyclo ./TouchGFX/generated/gui_generated/src/containers/PumpOptionBase.d ./TouchGFX/generated/gui_generated/src/containers/PumpOptionBase.o ./TouchGFX/generated/gui_generated/src/containers/PumpOptionBase.su ./TouchGFX/generated/gui_generated/src/containers/PumpSetupWidgetBase.cyclo ./TouchGFX/generated/gui_generated/src/containers/PumpSetupWidgetBase.d ./TouchGFX/generated/gui_generated/src/containers/PumpSetupWidgetBase.o ./TouchGFX/generated/gui_generated/src/containers/PumpSetupWidgetBase.su

.PHONY: clean-TouchGFX-2f-generated-2f-gui_generated-2f-src-2f-containers

