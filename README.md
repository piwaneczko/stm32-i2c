# STM32-I2C Template

Template for testing I2C communucation with ICP10125 barometer

Project imported to CMake with [https://github.com/MaJerle/stm32-cube-cmake-vscode](https://github.com/MaJerle/stm32-cube-cmake-vscode)

### Environment Path Changes

- C:\ST\STM32CubeIDE_<#version>\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.<#version>\tools\bin;
- C:\ST\STM32CubeIDE_<#version>\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.win32_2.<#version>\tools\bin;
- C:\ST\STM32CubeIDE_<#version>\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.<#version>\tools\bin;

### Add following environment variable to system

ST_CMSIS_SVD_DIR=C:\ST\STM32CubeIDE_<#version>\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.productdb.debug_2.<#version>\resources\cmsis\STMicroelectronics_CMSIS_SVD

## Notes

For IntelliSense issues copy directory
C:\ST\STM32CubeIDE_<#version>\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.<#version>\tools\arm-none-eabi\include\c++\10.3.1\arm-none-eabi\bits
to one directory up
C:\ST\STM32CubeIDE_<#version>\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.<#version>\tools\arm-none-eabi\include\c++\10.3.1,
because some files are missing.

## Linux packages

> $ sudo apt update  
> $ sudo apt install build-essential libncurses5 cmake

## Nucleo configuration

|     Name    | Pin  |
|-------------|------|
| I2C1_SCL    | PB8  |
| I2C1_SDA    | PB9  |
