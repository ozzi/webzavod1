################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Barrier.cpp \
../src/Buffer.cpp \
../src/Filesystem.cpp \
../src/Network.cpp \
../src/Params.cpp \
../src/Program.cpp \
../src/Thread.cpp \
../src/webzavod1.cpp 

OBJS += \
./src/Barrier.o \
./src/Buffer.o \
./src/Filesystem.o \
./src/Network.o \
./src/Params.o \
./src/Program.o \
./src/Thread.o \
./src/webzavod1.o 

CPP_DEPS += \
./src/Barrier.d \
./src/Buffer.d \
./src/Filesystem.d \
./src/Network.d \
./src/Params.d \
./src/Program.d \
./src/Thread.d \
./src/webzavod1.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


