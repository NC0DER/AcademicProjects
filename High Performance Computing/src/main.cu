#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "../include/matrix.h"
#include "../include/timer.h"
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

    double epsilon = pow(10, -6);
    double delta_norm, norm;
    double delta_norm_square, norm_square;
    float msec = 0.f;
    float avg_Msec = 0.f;
    double sec = 0.0;
    double avg_sec = 0.0;
    double total_msec = 0.0;
    int count = 0;

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

        return EXIT_FAILURE;
    }
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
    delta_norm = 0;
    norm = 0;
    
    do {
        ++count; // Count the amount of iterations.
        msec = Multiply(dev_M, dev_w, dev_x, dev_x_prev, dev_s, block_num); // Multiply and return the elapsed time.

#if debug
        std::cout << "Multiply time elapsed: " << msec << " msec\n";
#endif      
        avg_Msec += msec;
        // Transfer Xk, Xk-1 back to cpu in order to calculate the norm.
        cudaMemcpy(x.data, dev_x.data, dev_x.size * sizeof(double), cudaMemcpyDeviceToHost);
        cudaMemcpy(x_prev.data, dev_x_prev.data, dev_x_prev.size * sizeof(double), cudaMemcpyDeviceToHost);

        // Start Measuring - Calculate norm 
        start_timer
            // Calculate norm ^ 2 = sum(Xk[i] ^ 2) and norm
            norm_square = 0;
        for (int row = 0; row < x.rows; ++row) {
            norm_square += std::pow(x.data[row], 2);
        }
        norm = std::sqrt(norm_square);

        // Normalize Xk.
        for (int row = 0; row < x.rows; ++row) {
            x.data[row] /= norm;
        }

        // Calculate ||Xk - Xk-1|| ^ 2 and ||Xk - Xk-1||.
        delta_norm_square = 0;
        for (int row = 0; row < x_prev.size && x.size; ++row) {
            delta_norm_square += std::pow((x.data[row] - x_prev.data[row]), 2);
        }
        delta_norm = std::sqrt(delta_norm_square);
        // Stop Measuring - Norm calculated.
        stop_timer

            if (delta_norm > epsilon) {
                // Calculate the average time in seconds for norm calculation.
#ifdef _WIN32
                std::chrono::duration<double> elapsed_seconds = stop - start;
                sec = elapsed_seconds.count();
#else
                sec = (stop.tv_sec - start.tv_sec) + ((stop.tv_usec - start.tv_usec) / 1000000.0);
#endif
                avg_sec += sec;
                total_msec += (sec * 1000) + msec;
#if debug
                std::cout << "delta_norm " << delta_norm << "\n"
                          << "Elapsed seconds for Norm: " << sec << " sec \n"
                          << std::endl;
#endif
                // Transfer the current Xk (Host) to the new Xk-1 (Device)!!
                cudaMemcpy(dev_x_prev.data, x.data, dev_x.size * sizeof(double), cudaMemcpyHostToDevice);

                if (cudaStatus != cudaSuccess) {
                    std::cerr << "cudaMemcpy failed to swap xk and xk - 1!";

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

                // Re - initialize the new Xk to 0.
                initialize_vector(dev_x, 0, block_num);
            }
    } while (delta_norm > epsilon);

    // Print the final average times.
    avg_sec /= count;
    avg_Msec /= count;
#if debug
    std::cout << std::endl
              << "Average Multiplication time: " << avg_Msec << " msec \n"
              << "Average Norm Calculation time: " << avg_sec * 1000 << " msec \n"
              << "Average Iteration time: " << avg_Msec + (avg_sec * 1000) << " msec \n\n"
              << "Total Number of iterations: " << count << "\n"
              << "Total Time of all iterations: " << total_msec / 1000 << " sec" << std::endl;
#endif
    // Open output stream for file write only.
    std::ofstream result("results.csv");

    for (int row = 0; row < x.rows - 1; row++) {
        result << x.data[row] << ",";
    }
    // Unroll the last iteration of the loop above.
    result << x.data[x.rows - 1] << std::endl;
    // Close the file.
    result.close();
    
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