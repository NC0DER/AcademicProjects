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

    return EXIT_SUCCESS;
}