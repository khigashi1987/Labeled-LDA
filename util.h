/*
    util.h
*/
#ifndef UTIL_H
#define UTIL_H

extern char *strconcat(const char *s, const char *t);
extern void normalize_matrix_row(double **dst, double **src, int rows, int cols);
extern void normalize_matrix_col(double **dst, double **src, int rows, int cols);
#endif
