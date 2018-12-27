#ifndef MATRIX_H
#define MATRIX_H

#define idx(row, offset, col) (row * offset + col)
struct matrix {
    matrix(int rows, int cols) {
        this->rows = rows;
        this->cols = cols;
        this->size = rows * cols;
        this->data = NULL;
    }
    double *data;
    int rows;
    int cols;
    int size;
};
#endif