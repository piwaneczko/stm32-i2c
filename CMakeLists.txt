cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(PROJ_PATH ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: " ${CMAKE_BUILD_TYPE})

include(cmake/gcc-arm-none-eabi.cmake)

#
# Core project settings
#
project(stm32-i2c)
enable_language(C CXX ASM)

#
# Core MCU flags, CPU, instruction set and FPU setup
# Needs to be set properly for your MCU
#
set(CPU_PARAMETERS
    -mthumb

    # This needs attention to properly set for used MCU
    -mcpu=cortex-m4 # Modified
    -mfpu=fpv4-sp-d16 # Modified
    -mfloat-abi=hard # Modified
)

# Set linker script
set(linker_script_SRC ${PROJ_PATH}/STM32WB55RGVX_FLASH.ld)
set(EXECUTABLE ${CMAKE_PROJECT_NAME})

#
# List of source files to compile
#
set(sources_SRCS # Modified
    app/app.cpp
    app/devices/icp10125.cpp
    app/drivers/button.cpp
    app/drivers/gpio.cpp
    app/drivers/sci.cpp
    app/drivers/i2c.cpp
    app/drivers/timer.cpp
    app/math/crc/crc8maxim.cpp
    app/utilities/log.cpp
    app/utilities/performanceTimer.cpp
    Core/Src/main.c
    Core/Src/stm32wbxx_hal_msp.c
    Core/Src/stm32wbxx_it.c
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    Core/Src/system_stm32wbxx.c
    Core/Startup/startup_stm32wb55rgvx.s
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_cortex.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_dma_ex.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_dma.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_exti.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_flash_ex.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_flash.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_gpio.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_hsem.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_i2c_ex.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_i2c.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pcd_ex.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pcd.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pwr_ex.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pwr.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rcc_ex.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rcc.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_tim_ex.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_tim.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_uart_ex.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_uart.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_ll_rcc.c
    Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_ll_usb.c
)

#
# Include directories
#
set(include_path_DIRS # Modified
    app
    app/devices
    app/drivers
    app/utilities
    app/math/crc
    Core/Inc
    Drivers/STM32WBxx_HAL_Driver/Inc
    Drivers/STM32WBxx_HAL_Driver/Inc/Legacy
    Drivers/CMSIS/Device/ST/STM32WBxx/Include
    Drivers/CMSIS/Include
)

#
# Symbols definition
#
set(symbols_SYMB # Modified

    # Put here your symbols (preprocessor defines), one in each line
    # Encapsulate them with double quotes for safety purpose
    $<IF:$<CONFIG:Release>,"NDEBUG","DEBUG">
    "STM32WB55xx"
    "USE_HAL_DRIVER"
    "EXECUTABLE=\"${EXECUTABLE}\""
)

# Executable files
add_executable(${EXECUTABLE} ${sources_SRCS})

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${include_path_DIRS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-volatile>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-missing-field-initializers>

    # Full release/debug configuration
    $<IF:$<CONFIG:Release>,-O3, -O0 -g3 -ggdb>
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -u _printf_float # STDIO float formatting support
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)

file(WRITE build/flash.jlink
    "Loadfile " ${CMAKE_BINARY_DIR}/${EXECUTABLE}.bin " 0x08000000" \n
    "Verifybin " ${CMAKE_BINARY_DIR}/${EXECUTABLE}.bin " 0x08000000" \n
    Go\n
    Exit)

# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
)

# Convert to bin file -> add conditional check?
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)