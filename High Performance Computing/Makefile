NVCC_FLAGS = -arch=compute_20 -code=sm_20 -O3 --machine 64
EXECUTABLE =

fast: NVCC_FLAGS += -DSHARED
fast: EXECUTABLE = kernel_fast
fast: main

slow: EXECUTABLE = kernel_slow
slow: main

main: src/main.cu src/pddp.cu src/wrappers.cu
	nvcc main.cu pddp.cu wrappers.cu -o $(EXECUTABLE) $(NVCC_FLAGS)

clean:
	rm -f kernel_*
