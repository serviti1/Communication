################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/SecureLayer.c \
../source/aes.c \
../source/lwip_tcpecho_freertos.c \
../source/secureApplication.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/SecureLayer.d \
./source/aes.d \
./source/lwip_tcpecho_freertos.d \
./source/secureApplication.d \
./source/semihost_hardfault.d 

OBJS += \
./source/SecureLayer.o \
./source/aes.o \
./source/lwip_tcpecho_freertos.o \
./source/secureApplication.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K64F -DFREEDOM -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\source" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\mdio" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\phy" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\lwip\contrib\apps\tcpecho" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\lwip\port" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\lwip\src" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\lwip\src\include" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\drivers" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\utilities" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\device" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\component\uart" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\component\serial_manager" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\component\lists" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\CMSIS" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\freertos\freertos_kernel\include" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\nxf63523\Documents\Especialidad\Communication_workspace\frdmk64f_lwip_tcpecho_freertos\board" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/SecureLayer.d ./source/SecureLayer.o ./source/aes.d ./source/aes.o ./source/lwip_tcpecho_freertos.d ./source/lwip_tcpecho_freertos.o ./source/secureApplication.d ./source/secureApplication.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

