/*
    dmatrix.c
    an implementation of double matrix
*/
#include <stdlib.h>
#include "dmatrix.h"

double **dmatrix(int rows, int cols){
    double **matrix;
    int i;
    
    matrix = (double **)calloc(rows, sizeof(double *));
    if(matrix == NULL)
        return NULL;
    for(i = 0;i < rows;i++){
        matrix[i] = (double *)calloc(cols, sizeof(double));
        if(matrix[i] == NULL)
            return NULL;
    }
    
    return matrix;
}

void free_dmatrix(double **matrix, int rows){
    int i;
    for(i = 0;i < rows;i++){
        free(matrix[i]);
    }
    free(matrix);
}
