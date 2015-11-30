/*
    util.c
*/
#include <stdio.h>
#include <string.h>
#include "util.h"

char *strconcat(const char *s, const char *t){
    static char z[BUFSIZ];
    strcpy(z, s);
    strcat(z, t);
    return z;
}

void normalize_matrix_row(double **dst, double **src, int rows, int cols){
    int i, j;
    double z;
    
    for(i = 0;i < rows;i++){
        for(j = 0, z = 0;j < cols;j++)
            z += src[i][j];
        for(j = 0;j < cols;j++)
            dst[i][j] = src[i][j] / z;
    }
}

void normalize_matrix_col(double **dst, double **src, int rows, int cols){
    int i, j;
    double z;
    
    for(j = 0;j < cols;j++){
        for(i = 0, z = 0;i < rows;i++)
            z += src[i][j];
        for(i = 0;i < rows;i++)
            dst[i][j] = src[i][j] / z;
    }
}
