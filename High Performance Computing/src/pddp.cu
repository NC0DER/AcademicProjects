#include "../include/pddp.cuh"

__global__ void initialize_vector_kernel(matrix x, double value)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;

    if (index < x.rows) {
        x.data[index] = value;
    }
}