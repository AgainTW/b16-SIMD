# set var
build=$HOME/repo/rv32emu/build
rv32compile=rsicv-none-elf-gcc

opt=b16_SIMD_opt
norm=b16_SIMD
mat=mat_int32
level=( "-O0" "-O1" "-O2" "-O3" "-Ofast" )
lev=( "O0" "O1" "O2" "O3" "Ofast" )

# output
echo ${mat} output..
for (( i=0; i<=4; i=i+1 )); do
    ${build}/rv32emu ${mat}_${lev[$[i]]}
done

echo ${norm} output..
for (( i=0; i<=4; i=i+1 )); do
    ${build}/rv32emu ${norm}_${lev[$[i]]}
done

echo ${opt} output..
for (( i=0; i<=4; i=i+1 )); do
    ${build}/rv32emu ${opt}_${lev[$[i]]}
done

# ELF file header
echo ${mat} ELF file header output..
for (( i=0; i<=4; i=i+1 )); do
    ${build}/rv_histogram ${mat}_${lev[$[i]]}
done

echo ${norm} ELF file header output..
for (( i=0; i<=4; i=i+1 )); do
    ${build}/rv_histogram ${norm}_${lev[$[i]]}
done

echo ${opt} ELF file header output..
for (( i=0; i<=4; i=i+1 )); do
    ${build}/rv_histogram ${opt}_${lev[$[i]]}
done

echo all output done..