#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "../include/matrix.h"
#include "../include/wrappers.cuh"

#define debug 1

int main(int argc, char *argv[]) {
    // Filename given as input from console.
    if (argc != 3) {
        std::cerr << "Syntax: ./<executable> <filename> <blocksize per dimension> \n"
                  << " *Note: Some kernels use the same blocksize for dimension X and Y \n"
                  << " *Note: The resulting blocksize is (e.g for input n) n x n x 1 for each dim (x, y, z) \n \n"
                  << "**Note: Blocksize for each dimension X & Y much larger than 32 \n"
                  << "**Note: is not supported for cards with comp.capability < 6.0" << std::endl;
        return EXIT_FAILURE;
    }
    // Convert the text argument to integer.
    int block_num = 0;
    std::istringstream block_stream(argv[2]);
    block_stream >> block_num;
    
    // Bounds Checking for the blocksize.
    if (block_num < 0) {
        std::cerr << " *Error: blocksize for each dimension should be a positive number" << std::endl;
        return EXIT_FAILURE;
    }
    else if (block_num < 16) {
        std::cerr << " *Error: the number of threads for each dimension exceeds 65535 \n (e.g 1.000.000 / 15 > 65335) \n"
                  << " *Note:  use a blocksize for each dimension >= 16" << std::endl;
        return EXIT_FAILURE;
    }
    else if (block_num > 32) {
        std::cerr << " *Error: the number of blocks for both dimensions exceeds 1024 \n (e.g 33x33 = 1089) \n"
                  <<  " *Note:  use a blocksize for each dimension <= 32" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream file(argv[1]);
    // Checking if file stream has succesfully opened.
    if (!file.is_open()) {
        std::cerr << "File doesn't exist, or failed to open!" << std::endl;
        return EXIT_FAILURE;
    }

    // Load the float values on a dynamically expanding vector.
    std::vector<double> text;
    std::string line;
    double value = 0;
    int n = 0, m = 0;
    while (getline(file, line)) {
        std::stringstream stream(line);
        std::string col;
        m = 0;
        while (getline(stream, col, ',')) {
            std::istringstream num_stream(col);
            num_stream >> value;
            text.push_back(value);
            m++;
        }
        n++;
    }
    file.close();

    matrix M(n, m);
    matrix w(n, 1);
    matrix x(m, 1);
    matrix x_prev(m, 1);

    matrix dev_M(n, m);
    matrix dev_w(n, 1);
    matrix dev_x(m, 1);
    matrix dev_x_prev(m, 1);
    matrix dev_s(n, 1);

    M.data = (double *)malloc(M.size * sizeof(double));
    w.data = (double *)malloc(w.size * sizeof(double));
    x.data = (double *)malloc(x.size * sizeof(double));
    x_prev.data = (double *)malloc(x_prev.size * sizeof(double));

    // Load M on the Host system memory.
    for (int row = 0; row < M.rows; ++row) {
        for (int col = 0; col < M.cols; ++col) {
            M.data[idx(row, M.cols, col)] = text[idx(row, M.cols, col)];
        }
    }
    text.clear(); // Empty the vector.

    // Initialize w values to 0
    for (int index = 0; index < n; ++index) {
        w.data[index] = 0.0;
    }

    cudaError_t cudaStatus; // Return value of each cuda call.

    // Choose which GPU Device to execute on.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        std::cerr << "cudaSetDevice failed!";

        // Free all memory both in Device and CPU.
        cudaFree(dev_M.data);
        cudaFree(dev_w.data);
        cudaFree(dev_x.data);
        cudaFree(dev_x_prev.data);

        free(M.data);
        free(w.data);
        free(x.data);
        free(x_prev.data);

        return EXIT_FAILURE;
    }

    cudaMalloc(&dev_M.data, dev_M.size * sizeof(double));
    cudaMalloc(&dev_w.data, dev_w.size * sizeof(double));
    cudaMalloc(&dev_x.data, dev_x.size * sizeof(double));
    cudaMalloc(&dev_x_prev.data, dev_x_prev.size * sizeof(double));
    cudaMalloc(&dev_s.data, dev_s.size * sizeof(double));

    initialize_vector(dev_x, 0, block_num);
    initialize_vector(dev_x_prev, 1, block_num);

    cudaStatus = cudaMemcpy(dev_M.data, M.data, dev_M.size * sizeof(double), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        std::cerr << "cudaMemcpy failed! 1";

        // Free all memory both in Device and CPU.
        cudaFree(dev_M.data);
        cudaFree(dev_w.data);
        cudaFree(dev_x.data);
        cudaFree(dev_x_prev.data);

        free(M.data);
        free(w.data);
        free(x.data);
        free(x_prev.data);

    initialize_w(dev_M, dev_w, block_num);
    cudaStatus = cudaMemcpy(w.data, dev_w.data, dev_w.size * sizeof(double), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        std::cerr << "cudaMemcpy failed! 2";

        // Free all memory both in Device and CPU.
        cudaFree(dev_M.data);
        cudaFree(dev_w.data);
        cudaFree(dev_x.data);
        cudaFree(dev_x_prev.data);

        free(M.data);
        free(w.data);
        free(x.data);
        free(x_prev.data);

        return EXIT_FAILURE;
    }

        return EXIT_FAILURE;
    }
    cudaFree(dev_M.data);
    cudaFree(dev_w.data);
    cudaFree(dev_x.data);
    cudaFree(dev_x_prev.data);
    cudaFree(dev_s.data);

    free(M.data);
    free(w.data);
    free(x.data);
    free(x_prev.data);
    return EXIT_SUCCESS;
}