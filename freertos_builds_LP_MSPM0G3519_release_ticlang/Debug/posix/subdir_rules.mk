################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
posix/PTLS.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/PTLS.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/aeabi_portable.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/aeabi_portable.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/clock.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/clock.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/memory.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/memory.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/mqueue.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/mqueue.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/pthread.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/pthread.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/pthread_barrier.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/pthread_barrier.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/pthread_cond.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/pthread_cond.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/pthread_mutex.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/pthread_mutex.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/pthread_rwlock.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/pthread_rwlock.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/sched.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/sched.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/semaphore.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/semaphore.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/sleep.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/sleep.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

posix/timer.o: /home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/freertos/timer.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -I"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/third_party/CMSIS/Core/Include" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/posix/ticlang" -I"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -I"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/include" -D__MSPM0G3519__ -gdwarf-3 -MMD -MP -MF"posix/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


