#include "../include/pddp.cuh"

#ifdef SHARED
__global__ void matrix_multiplication_kernel(matrix w, matrix a, matrix b, matrix c, unsigned int tile_size)
{
    int bx = blockIdx.x;
    int by = blockIdx.y;

    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int row = by * blockDim.y + ty;
    int col = bx * blockDim.x + tx;

    extern __shared__ double buffer[];
    double *a_shared = &buffer[0];
    double *b_shared = &buffer[tile_size * tile_size];

    double sum = 0;

    for (int k = 0; k < (tile_size + a.cols - 1) / tile_size; k++) {
        if (k * tile_size + tx < a.cols && row < a.rows) {
            a_shared[ty * tile_size + tx] = a.data[row * a.cols + k * tile_size + tx];
        }
        else {
            a_shared[ty * tile_size + tx] = 0.0;
        }
        if (k * tile_size + ty < b.rows && col < b.rows) {
            b_shared[ty * tile_size + tx] = b.data[(k * tile_size + ty) * b.cols + col];
        }
        else {
            b_shared[ty * tile_size + tx] = 0.0;
        }
        __syncthreads();
#pragma unroll
        for (int n = 0; n < tile_size; ++n) {
            sum += (a_shared[ty * tile_size + n] - w.data[row]) * b_shared[n * tile_size + tx];
        }
        __syncthreads();
    }
    if (row < c.rows && col < c.cols) {
        c.data[row * c.cols + col] = sum;
    }
}

__global__ void transposed_matrix_multiplication_kernel(matrix w, matrix a, matrix b, matrix c, unsigned int tile_size)
{
    int bx = blockIdx.x; 
    int tx = threadIdx.x;
    int index = bx * blockDim.x + tx;

    extern __shared__ double shared[];
    
    double sum = 0;

    for (int k = 0; k < (tile_size + a.rows - 1) / tile_size; k++) {
        if (k * tile_size + tx < b.rows) {
            shared[tx] = b.data[k * tile_size + tx];
        }
        else {
            shared[tx] = 0.0;
        }
        __syncthreads();
#pragma unroll
        for (int n = 0; n < tile_size; ++n) {
            if (index + (n + tile_size * k) * a.cols < a.size) {
                sum += (a.data[index + (n + tile_size * k) * a.cols] - w.data[n + tile_size * k]) * shared[n];
            }
        }
        __syncthreads();
    }
    if (index < c.rows) {
        c.data[index] = sum;
    }
}
#else
__global__ void matrix_multiplication_kernel(matrix w, matrix a, matrix b, matrix c)
{
    int bx = blockIdx.x;
    int by = blockIdx.y;

    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int row = by * blockDim.y + ty;
    int col = bx * blockDim.x + tx;

    if (row < c.rows && col < c.cols) {
        double sum = 0;
#pragma unroll
        for (int k = 0; k < a.cols; k++) { //Common Dimension -> m 
            sum += (a.data[row * a.cols + k] - w.data[row]) * b.data[k * b.cols + col];
        }
        c.data[row * c.cols + col] = sum;
    }
}

__global__ void transposed_matrix_multiplication_kernel(matrix w, matrix a, matrix b, matrix c)
{
    int bx = blockIdx.x;
    int by = blockIdx.y;

    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int row = by * blockDim.y + ty;
    int col = bx * blockDim.x + tx;

    if (row < c.rows && col < c.cols) {
        double sum = 0;
#pragma unroll
        for (int k = 0; k < a.rows; k++) { //Common Dimension -> n 
            sum += (a.data[k * a.cols + row] - w.data[k]) * b.data[k * b.cols + col];
        }
        c.data[row * c.cols + col] = sum;
    }
}
#endif

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