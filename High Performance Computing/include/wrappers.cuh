#ifndef WRAPPERS_H
#define WRAPPERS_H

#include "pddp.cuh"
void initialize_vector(matrix x, double value, unsigned int blockSizeX);
void initialize_w(matrix M, matrix w, unsigned int blockSizeX);
float matrix_multiplication(matrix w, matrix M, matrix x_prev, matrix s, unsigned int blockSize);
float transposed_matrix_multiplication(matrix w, matrix M, matrix s, matrix x, unsigned int blockSize);
float Multiply(matrix M, matrix w, matrix x, matrix x_prev, matrix s, unsigned int blockSize);
#endif