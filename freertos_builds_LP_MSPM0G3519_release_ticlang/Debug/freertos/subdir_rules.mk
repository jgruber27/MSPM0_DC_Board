################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
freertos/croutine.o: /home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/croutine.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"freertos/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/event_groups.o: /home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/event_groups.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"freertos/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/heap_4.o: /home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/MemMang/heap_4.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"freertos/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/list.o: /home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/list.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"freertos/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/queue.o: /home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/queue.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"freertos/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/stream_buffer.o: /home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/stream_buffer.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"freertos/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/tasks.o: /home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/tasks.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"freertos/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/timers.o: /home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/timers.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"freertos/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


