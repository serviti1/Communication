################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../phy/fsl_phyksz8081.c 

C_DEPS += \
./phy/fsl_phyksz8081.d 

OBJS += \
./phy/fsl_phyksz8081.o 


# Each subdirectory must supply rules for building sources it contributes
phy/%.o: ../phy/%.c phy/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -D_POSIX_SOURCE -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K64F -DFREEDOM -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\source" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\mdio" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\phy" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\lwip\src\include\lwip\apps" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\lwip\port" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\lwip\src" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\lwip\src\include" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\drivers" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\utilities" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\device" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\component\uart" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\component\serial_manager" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\component\lists" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\CMSIS" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\freertos\freertos_kernel\include" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\jesus\Documents\MCUXpressoIDE_11.6.0_8187\workspace\frdmk64f_Practice2_lwip_mqtt_freertos\board" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-phy

clean-phy:
	-$(RM) ./phy/fsl_phyksz8081.d ./phy/fsl_phyksz8081.o

.PHONY: clean-phy

