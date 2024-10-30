# Git
## for ssh
```
git clone -b stm32_w5500_makefile git@github.com:p1anx/STM32-HAL-W5500.git
```
## for url
```
git clone -b stm32_w5500_makefile https://github.com/p1anx/STM32-HAL-W5500.git
```
# Platformio
`note:`firmware version must be 1.8.5.
## .ini
```ini
[platformio]
src_dir = ./
[env:genericSTM32F103ZE]
platform = ststm32
board = genericSTM32F103ZE
framework = stm32cube

monitor_speed = 115200
upload_protocol = cmsis-dap
; build_src_filter = +<Core/Src> +<BSP> +<BSP/W5500> +<Middlewares/Third_Party/FreeRTOS/Source> +<Middlewares/Third_Party/FreeRTOS/Source/portable>
build_src_filter = +<Core/Src> +<BSP> +<Middlewares>
build_flags = 
    -ICore/Inc
    -IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    -IMiddlewares/Third_Party/FreeRTOS/Source/include
    -IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3
    -IBSP/W5500
    -IBSP
```
