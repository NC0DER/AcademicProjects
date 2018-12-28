#include <iostream>
#include "../include/wrappers.cuh"

void initialize_vector(matrix x, double value, unsigned int blockSizeX)
{
    cudaError_t cudaStatus;
    // Define grid dimensions
    unsigned int numBlocksX = (x.rows - 1) / blockSizeX + 1;
    dim3 dimGrid(numBlocksX, 1, 1);
    dim3 dimBlock(blockSizeX, 1, 1);
    initialize_vector_kernel << <dimGrid, dimBlock >> > (x, value);

    // cudaDeviceSynchronize waits for the kernel to finish.
    // While returing any errors that happended during the kernel launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        std::cerr << "cudaDeviceSynchronize returned error code " << cudaStatus << " after launching initialize_vector Kernel!" << std::endl;
    }
}

void initialize_w(matrix M, matrix w, unsigned int blockSizeX)
{
    cudaError_t cudaStatus;
    // Define grid dimensions
    dim3 dimBlock;
    dim3 dimGrid;
    dimBlock.x = blockSizeX;
    dimBlock.y = 1;
    dimBlock.z = 1;
    dimGrid.x = (M.rows - 1) / dimBlock.x + 1;
    dimGrid.y = 1;
    dimGrid.z = 1;
    initialize_w_kernel << <dimGrid, dimBlock >> > (M, w);

    // cudaDeviceSynchronize waits for the kernel to finish.
    // While returing any errors that happended during the kernel launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        std::cerr << "cudaDeviceSynchronize returned error code " << cudaStatus << " after launching initialize_w Kernel!" << std::endl;
    }
}

float matrix_multiplication(matrix w, matrix M, matrix x_prev, matrix s, unsigned int blockSize)
{
    cudaError_t cudaStatus;
    // Events for measuring the execution time
    cudaEvent_t start;
    cudaEvent_t stop;
    float msec;

    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // Define grid dimensions
    dim3 dimBlock;
    dim3 dimGrid;
    dimBlock.x = blockSize;
    dimBlock.y = blockSize;
    dimBlock.z = 1;
    dimGrid.x = (s.cols - 1) / dimBlock.x + 1;
    dimGrid.y = (s.rows - 1) / dimBlock.y + 1;
    dimGrid.z = 1;

    initialize_vector(s, 0, 16);
    cudaEventRecord(start); // Start Event.
#ifdef SHARED  // tileSize = blockSize
    matrix_multiplication_kernel << <dimGrid, dimBlock, 2 * blockSize * blockSize * sizeof(double) >> >(w, M, x_prev, s, blockSize); 
#else
    matrix_multiplication_kernel << <dimGrid, dimBlock, 2 * blockSize * blockSize * sizeof(double) >> >(w, M, x_prev, s);
#endif
    cudaEventRecord(stop); // Stop Event.

    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&msec, start, stop);

    // cudaDeviceSynchronize waits for the kernel to finish.
    // While returing any errors that happended during the kernel launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        std::cerr << "cudaDeviceSynchronize returned error code " << cudaStatus << " after launching MatrixVectorMult Kernel!" << std::endl;
    }
    return msec;
}

float transposed_matrix_multiplication(matrix w, matrix M, matrix s, matrix x, unsigned int blockSize)
{
    cudaError_t cudaStatus;
    // Events for measuring the execution time
    cudaEvent_t start;
    cudaEvent_t stop;
    float msec;

    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // Define grid dimensions
    dim3 dimBlock;
    dim3 dimGrid;
    
    dimBlock.x = blockSize;
    dimBlock.y = blockSize;
    dimBlock.z = 1;
    dimGrid.z = 1;
    dimGrid.x = (x.cols - 1) / dimBlock.x + 1;
    dimGrid.y = (x.rows - 1) / dimBlock.y + 1;
    dimGrid.z = 1;

    cudaEventRecord(start); // Start Event.
    transposed_matrix_multiplication_kernel << <dimGrid, dimBlock >> >(w, M, s, x);
    cudaEventRecord(stop); // Stop Event.

    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&msec, start, stop);

    // cudaDeviceSynchronize waits for the kernel to finish.
    // While returing any errors that happended during the kernel launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        std::cerr << "cudaDeviceSynchronize returned error code " << cudaStatus << " after launching TransposedMatrixVectorMult Kernel!" << std::endl;
    }
    return msec;
}

float Multiply(matrix M, matrix w, matrix x, matrix x_prev, matrix s, unsigned int blockSize)
{
    float msec[1];
    msec[0] = matrix_multiplication(w, M, x_prev, s, blockSize); // s = (M - w * eT) * x_prev
    msec[1] = transposed_matrix_multiplication(w, M, s, x, blockSize); // (M - w * eT)T * s
    return msec[0] + msec[1]; // Return the total elapsed time of 2 multiplication kernels.
}