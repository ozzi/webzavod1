################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Params.cpp \
../src/Params_test.cpp \
../src/Program.cpp \
../src/webzavod1.cpp 

OBJS += \
./src/Params.o \
./src/Params_test.o \
./src/Program.o \
./src/webzavod1.o 

CPP_DEPS += \
./src/Params.d \
./src/Params_test.d \
./src/Program.d \
./src/webzavod1.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


