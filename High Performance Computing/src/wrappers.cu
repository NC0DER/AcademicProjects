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
