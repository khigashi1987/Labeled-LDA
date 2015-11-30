/*
    writer.c
    an implementation of matrix writer
*/
#include <stdio.h>
#include "writer.h"

void llda_write(FILE *pp, FILE *tp, double **phi, double **theta, int nclass, int nlex, int ndoc){
    printf("writing model...\n"); fflush(stdout);
    write_matrix(pp, phi, nlex, nclass);
    write_matrix(tp, theta, ndoc, nclass);
    printf("done.\n"); fflush(stdout);
}

void write_matrix(FILE *fp, double **matrix, int rows, int cols){
    int i, j;
    for(i = 0;i < rows;i++)
        for(j = 0;j < cols;j++)
            fprintf(fp, "%.7e%s", matrix[i][j], (j == cols - 1)? "\n":" ");
}
