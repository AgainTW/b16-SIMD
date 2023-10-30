# set var
build=$HOME/repo/rv32emu/build
rv32compile=riscv-none-elf-gcc

d=b16_D_SIMD_opt
v=b16_V_SIMD_opt
m=b16_M_SIMD_opt
level=( "-O0" "-O3" )
lev=( "O0" "O3" )

# output
echo ${d} output..
for (( i=0; i<=1; i=i+1 )); do
    ${build}/rv32emu ${d}_${lev[$[i]]}.elf
done

echo ${v} output..
for (( i=0; i<=1; i=i+1 )); do
    ${build}/rv32emu ${v}_${lev[$[i]]}.elf
done

echo ${m} output..
for (( i=0; i<=1; i=i+1 )); do
    ${build}/rv32emu ${m}_${lev[$[i]]}.elf
done

echo all output done..