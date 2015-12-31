/*
    writer.c
    an implementation of matrix writer
*/
#include <stdio.h>
#include "writer.h"

void llda_write(FILE *pp, FILE *tp, FILE *n_mzp, FILE *n_wzp, double **phi, double **theta, int **n_mz, int **n_zw, int nclass, int nlex, int ndoc){
    printf("writing model...\n"); fflush(stdout);
    write_matrix(pp, phi, nlex, nclass);
    write_matrix(tp, theta, ndoc, nclass);
    write_n_mz(n_mzp, n_mz, ndoc, nclass);
    write_n_zw(n_wzp, n_zw, nclass, nlex);
    printf("done.\n"); fflush(stdout);
}

void write_matrix(FILE *fp, double **matrix, int rows, int cols){
    int i, j;
    for(i = 0;i < rows;i++)
        for(j = 0;j < cols;j++)
            fprintf(fp, "%.7e%s", matrix[i][j], (j == cols - 1)? "\n":" ");
}

void write_n_mz(FILE *fp, int **n_mz, int rows, int cols){
    int i, j;
    for(i = 0;i < rows;i++)
        for(j = 0;j < cols;j++)
            fprintf(fp, "%d%s", n_mz[i][j], (j == cols - 1)? "\n":" ");
}

void write_n_zw(FILE *fp, int **n_zw, int rows, int cols){
    int i, j;
    for(j = 0;j < cols;j++)
        for(i = 0;i < rows;i++)
            fprintf(fp, "%d%s", n_zw[i][j], (i == rows -1)? "\n":" ");
}
