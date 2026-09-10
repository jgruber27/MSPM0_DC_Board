################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/home/jim/workspace_ccstheia/MSPM0_DC_Board" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -D__MSPM0G3519__ -g -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


