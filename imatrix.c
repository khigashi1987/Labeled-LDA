/*
    imatrix.c
    an implementation of int matrix
*/
#include <stdlib.h>
#include "imatrix.h"

int **imatrix(int rows, int cols){
    int **matrix;
    int i;
    
    matrix = (int **)calloc(rows, sizeof(int *));
    if(matrix == NULL)
        return NULL;
    for(i = 0;i < rows;i++){
        matrix[i] = (int *)calloc(cols, sizeof(int));
        if(matrix[i] == NULL)
            return NULL;
    }
    
    return matrix;
}

void free_imatrix(int **matrix, int rows){
    int i;
    for(i = 0;i < rows;i++){
        free(matrix[i]);
    }
    free(matrix);
}
