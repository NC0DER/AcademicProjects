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
    
    return EXIT_SUCCESS;
}