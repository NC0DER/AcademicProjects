#ifndef PDDP_H
#define PDDP_H

#include "matrix.h"
#ifdef SHARED
    __global__ void matrix_multiplication_kernel(matrix w, matrix a, matrix b, matrix c, unsigned int tile_size);
    __global__ void transposed_matrix_multiplication_kernel(matrix w, matrix a, matrix b, matrix c, unsigned int tile_size);
#else
    __global__ void matrix_multiplication_kernel(matrix w, matrix a, matrix b, matrix c);
    __global__ void transposed_matrix_multiplication_kernel(matrix w, matrix a, matrix b, matrix c);
#endif
__global__ void initialize_w_kernel(matrix M, matrix w);
__global__ void initialize_vector_kernel(matrix x, double value);
#endif