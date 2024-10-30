#!/bin/bash
if [ ! -f "commandfile.jlink" ]; then
    touch commandfile.jlink
    tee commandfile.jlink << EOF
device STM32F103ZE
si SWD
speed 4000
r
loadfile /home/xwj/1_code/stm32/w5500/build/test2.bin 0x08000000
q
EOF
fi
make
JLinkExe -CommandFile ./commandfile.jlink
