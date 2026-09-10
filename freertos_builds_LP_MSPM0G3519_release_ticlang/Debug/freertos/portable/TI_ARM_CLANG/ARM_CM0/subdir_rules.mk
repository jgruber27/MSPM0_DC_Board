################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
freertos/portable/TI_ARM_CLANG/ARM_CM0/port.o: /home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0/port.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"freertos/portable/TI_ARM_CLANG/ARM_CM0/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/portable/TI_ARM_CLANG/ARM_CM0/portasm.o: /home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0/portasm.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"freertos/portable/TI_ARM_CLANG/ARM_CM0/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


