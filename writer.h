/*
    writer.h
    a header file of matrix writer
*/
#ifndef WRITER_H
#define WRITER_H
#include <stdio.h>

extern void llda_write(FILE *pp, FILE *tp, FILE *n_mzp, FILE *n_wzp, double **phi, double **theta, int **n_mz, int **n_zw, int nclass, int nlex, int ndoc);
void write_matrix(FILE *fp, double **matrix, int rows, int cols);
void write_n_mz(FILE *fp, int **n_mz, int rows, int cols);
void write_n_zw(FILE *fp, int **n_zw, int rows, int cols);
#endif
