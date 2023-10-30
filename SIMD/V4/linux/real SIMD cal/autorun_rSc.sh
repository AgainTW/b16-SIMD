#!/bin/bash

# set var
rv32emu=$HOME/repo/rv32emu/build/rv32emu
rv32compile=riscv-none-elf-gcc
arch=rv32i_zicsr
abi=ilp32
gs=getcycles

d=b16_D_SIMD_opt
v=b16_V_SIMD_opt
m=b16_M_SIMD_opt
level=( "-O0" "-O3" )
lev=( "O0" "O3" )

# build env var
cd $HOME/${rv32compile}
source setenv
cd $HOME

# compile getcycle.s
cd $HOME/repo/rv32emu/tests/ag
${rv32compile} -march=${arch} -mabi=${abi} -c -o ${gs}.o ${gs}.s

# compile function
for (( i=0; i<=1; i=i+1 )); do
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o ${d}_${lev[${i}]}.o ${d}.c
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o ${v}_${lev[${i}]}.o ${v}.c
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o ${m}_${lev[${i}]}.o ${m}.c
    echo function compile done..
done

# compile main
for (( i=0; i<=1; i=i+1 )); do
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o main_${d}_${lev[${i}]}.o main_${d}.c
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o main_${v}_${lev[${i}]}.o main_${v}.c
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o main_${m}_${lev[${i}]}.o main_${m}.c
    echo main compile done..
done

# build elf
for (( i=0; i<=1; i=i+1 )); do
    ${rv32compile} -o ${d}_${lev[${i}]}.elf main_${d}_${lev[${i}]}.o ${d}_${lev[${i}]}.o ${gs}.o
    ${rv32compile} -o ${v}_${lev[${i}]}.elf main_${v}_${lev[${i}]}.o ${v}_${lev[${i}]}.o ${gs}.o
    ${rv32compile} -o ${m}_${lev[${i}]}.elf main_${m}_${lev[${i}]}.o ${m}_${lev[${i}]}.o ${gs}.o
    echo elf build done..
done

echo all compile done..