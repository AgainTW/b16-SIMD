riscv-none-elf-gcc -march=rv32i -mabi=ilp32 -O0 mat_int32.c -o mat_int32_O0.elf
riscv-none-elf-gcc -march=rv32i -mabi=ilp32 -O1 mat_int32.c -o mat_int32_O1.elf
riscv-none-elf-gcc -march=rv32i -mabi=ilp32 -O2 mat_int32.c -o mat_int32_O2.elf
riscv-none-elf-gcc -march=rv32i -mabi=ilp32 -O3 mat_int32.c -o mat_int32_O3.elf
riscv-none-elf-gcc -march=rv32i -mabi=ilp32 -Ofast mat_int32.c -o mat_int32_fast.elf

../../build/re32emu -at mat_int32_O0.elf
../../build/re32emu -at mat_int32_O1.elf
../../build/re32emu -at mat_int32_O2.elf
../../build/re32emu -at mat_int32_O3.elf
../../build/re32emu -at mat_int32_fast.elf
