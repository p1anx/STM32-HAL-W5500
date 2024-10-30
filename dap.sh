#!/bin/bash
make
openocd -f interface/cmsis-dap.cfg -f target/stm32f1x.cfg -c "program build/test2.bin 0x08000000 verify reset exit;"
