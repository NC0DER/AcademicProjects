#include "../include/pddp.cuh"

__global__ void initialize_w_kernel(matrix M, matrix w)
{
    int row = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < M.rows) {
        double sum = 0;
        for (size_t col = 0; col < M.cols; ++col) {
            sum += M.data[idx(row, M.cols, col)];
        }
        w.data[row] = sum / M.cols;
    }

}

__global__ void initialize_vector_kernel(matrix x, double value)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;

    if (index < x.rows) {
        x.data[index] = value;
    }
}