#!/bin/bash

# set var
rv32emu=$HOME/repo/rv32emu/build/rv32emu
rv32compile=riscv-none-elf-gcc
arch=rv32i_zicsr
abi=ilp32
gs=getcycles

opt=b16_SIMD_opt
norm=b16_SIMD
mat=mat_int32
level=( "-O0" "-O1" "-O2" "-O3" "-Ofast" )
lev=( "O0" "O1" "O2" "O3" "Ofast" )

# build env var
cd $HOME/${rv32compile}
source setenv
cd $HOME

# compile getcycle.s
cd $HOME/repo/rv32emu/tests/ag
${rv32compile} -march=${arch} -mabi=${abi} -c -o ${gs}.o ${gs}.s

# compile function
for (( i=0; i<=4; i=i+1 )); do
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o ${mat}_${lev[${i}]}.o ${mat}.c
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o ${norm}_${lev[${i}]}.o ${norm}.c
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o ${opt}_${lev[${i}]}.o ${opt}.c
    echo function compile done..
done

# compile main
for (( i=0; i<=4; i=i+1 )); do
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o main_${mat}_${lev[${i}]}.o main_${mat}.c
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o main_${norm}_${lev[${i}]}.o main_${norm}.c
    ${rv32compile} -march=${arch} -mabi=${abi} ${level[${i}]} -c -o main_${opt}_${lev[${i}]}.o main_${opt}.c
    echo main compile done..
done

# build elf
for (( i=0; i<=4; i=i+1 )); do
    ${rv32compile} -o ${mat}_${lev[${i}]}.elf main_${mat}_${lev[${i}]}.o ${mat}_${lev[${i}]}.o ${gs}.o
    ${rv32compile} -o ${norm}_${lev[${i}]}.elf main_${norm}_${lev[${i}]}.o ${norm}_${lev[${i}]}.o ${gs}.o
    ${rv32compile} -o ${opt}_${lev[${i}]}.elf main_${opt}_${lev[${i}]}.o ${opt}_${lev[${i}]}.o ${gs}.o
    echo elf build done..
done

echo all compile done..