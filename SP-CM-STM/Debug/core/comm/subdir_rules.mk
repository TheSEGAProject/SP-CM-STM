################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
core/comm/comm.obj: ../core/comm/comm.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmsp -g -O0 --define=__MSP430F235__ --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="core/comm/comm.pp" --obj_directory="core/comm" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


