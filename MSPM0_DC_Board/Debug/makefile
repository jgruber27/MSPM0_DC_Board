################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ad5672r.o" \
"./app_tasks.o" \
"./feedback_adc.o" \
"./main.o" \
"./main_freertos.o" \
"./terminal.o" \
"./ti_drivers_config.o" \
"./ti_msp_dl_config.o" \
"./freertos/ticlang/startup_mspm0g351x_ticlang.o" \
"../mspm0g3519.cmd" \
$(GEN_CMDS__FLAG) \
-Wl,-l"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/drivers/lib/ticlang/m0p/drivers_mspm0g151x_g351x.a" \
-Wl,-lfreertos_builds_LP_MSPM0G3519_release_ticlang.lib \
-Wl,-l"/home/jim/ti/mspm0_sdk_2_11_00_07/source/ti/driverlib/lib/ticlang/m0p/mspm0gx51x/driverlib.a" \
-Wl,-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include freertos/ticlang/subdir_vars.mk
-include subdir_rules.mk
-include freertos/ticlang/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
MSPM0_DC_Board.out 

EXE_OUTPUTS__QUOTED += \
"MSPM0_DC_Board.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "MSPM0_DC_Board.out"

dependents:
	-cd /home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang/Debug && $(MAKE) all

# Tool invocations
MSPM0_DC_Board.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) /home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang/Debug/freertos_builds_LP_MSPM0G3519_release_ticlang.lib
	@echo 'Arm Linker - building target: "$@"'
	"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang" -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -D__MSPM0G3519__ -g -Wall -Wl,-m"MSPM0_DC_Board.map" -Wl,-i"/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang/Debug" -Wl,-i"/home/jim/ti/mspm0_sdk_2_11_00_07/source" -Wl,-i"/home/jim/workspace_ccstheia/MSPM0_DC_Board" -Wl,-i"/home/jim/ti/ccs2101/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="MSPM0_DC_Board_linkInfo.xml" -Wl,--rom_model -o "MSPM0_DC_Board.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "ad5672r.o" "app_tasks.o" "feedback_adc.o" "main.o" "main_freertos.o" "terminal.o" "ti_drivers_config.o" "ti_msp_dl_config.o" "freertos/ticlang/startup_mspm0g351x_ticlang.o" 
	-$(RM) "ad5672r.d" "app_tasks.d" "feedback_adc.d" "main.d" "main_freertos.d" "terminal.d" "ti_drivers_config.d" "ti_msp_dl_config.d" "freertos/ticlang/startup_mspm0g351x_ticlang.d" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/jim/workspace_ccstheia/freertos_builds_LP_MSPM0G3519_release_ticlang/Debug/freertos_builds_LP_MSPM0G3519_release_ticlang.lib:

-include ../makefile.targets

