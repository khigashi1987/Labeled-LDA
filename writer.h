/*
    writer.h
    a header file of matrix writer
*/
#ifndef WRITER_H
#define WRITER_H
#include <stdio.h>

extern void llda_write(FILE *pp, FILE *tp, double **phi, double **theta, int nclass, int nlex, int ndoc);
void write_matrix(FILE *fp, double **matrix, int rows, int cols);
#endif
