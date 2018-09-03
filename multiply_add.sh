#!/bin/bash

# number of threads
for t in 1024 4096 16348 65536 262144 1048576 4194304
do
    # number of subdivisions:
    for n in 4 64 128 512 1024
    do
	let g=$t/$n
        g++ -DNUM_ELEMENTS=$t -DLOCAL_SIZE=$n -DNUM_WORK_GROUPS=$g -o multiply_add multiply_add.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp
        ./multiply_add
    done
done
rm multiply_add