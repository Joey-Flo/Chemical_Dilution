################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX/gui/src/cleansystem_screen/CleanSystemPresenter.cpp \
../TouchGFX/gui/src/cleansystem_screen/CleanSystemView.cpp 

OBJS += \
./TouchGFX/gui/src/cleansystem_screen/CleanSystemPresenter.o \
./TouchGFX/gui/src/cleansystem_screen/CleanSystemView.o 

CPP_DEPS += \
./TouchGFX/gui/src/cleansystem_screen/CleanSystemPresenter.d \
./TouchGFX/gui/src/cleansystem_screen/CleanSystemView.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX/gui/src/cleansystem_screen/%.o TouchGFX/gui/src/cleansystem_screen/%.su TouchGFX/gui/src/cleansystem_screen/%.cyclo: ../TouchGFX/gui/src/cleansystem_screen/%.cpp TouchGFX/gui/src/cleansystem_screen/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TouchGFX-2f-gui-2f-src-2f-cleansystem_screen

clean-TouchGFX-2f-gui-2f-src-2f-cleansystem_screen:
	-$(RM) ./TouchGFX/gui/src/cleansystem_screen/CleanSystemPresenter.cyclo ./TouchGFX/gui/src/cleansystem_screen/CleanSystemPresenter.d ./TouchGFX/gui/src/cleansystem_screen/CleanSystemPresenter.o ./TouchGFX/gui/src/cleansystem_screen/CleanSystemPresenter.su ./TouchGFX/gui/src/cleansystem_screen/CleanSystemView.cyclo ./TouchGFX/gui/src/cleansystem_screen/CleanSystemView.d ./TouchGFX/gui/src/cleansystem_screen/CleanSystemView.o ./TouchGFX/gui/src/cleansystem_screen/CleanSystemView.su

.PHONY: clean-TouchGFX-2f-gui-2f-src-2f-cleansystem_screen

