################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.c 

C_DEPS += \
./freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.d 

OBJS += \
./freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.o 


# Each subdirectory must supply rules for building sources it contributes
freertos/freertos_kernel/portable/GCC/ARM_CM4F/%.o: ../freertos/freertos_kernel/portable/GCC/ARM_CM4F/%.c freertos/freertos_kernel/portable/GCC/ARM_CM4F/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K64F -DFREEDOM -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\source" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\mdio" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\phy" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\lwip\contrib\apps\tcpecho" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\lwip\port" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\lwip\src" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\lwip\src\include" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\drivers" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\utilities" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\device" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\component\uart" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\component\serial_manager" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\component\lists" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\CMSIS" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\freertos\freertos_kernel\include" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\board" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-freertos-2f-freertos_kernel-2f-portable-2f-GCC-2f-ARM_CM4F

clean-freertos-2f-freertos_kernel-2f-portable-2f-GCC-2f-ARM_CM4F:
	-$(RM) ./freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.d ./freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.o

.PHONY: clean-freertos-2f-freertos_kernel-2f-portable-2f-GCC-2f-ARM_CM4F

