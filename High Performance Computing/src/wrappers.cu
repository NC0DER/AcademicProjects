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
