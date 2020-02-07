################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../adc_temp.cpp \
../daemon.cpp \
../main.cpp \
../server.cpp \
../timer.cpp 

OBJS += \
./adc_temp.o \
./daemon.o \
./main.o \
./server.o \
./timer.o 

CPP_DEPS += \
./adc_temp.d \
./daemon.d \
./main.d \
./server.d \
./timer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


