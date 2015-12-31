/*
    learn.h
*/
#ifndef LEARN_H
#define LEARN_H
#include <stdlib.h>
#include "feature.h"

#define RANDOM ((double)rand()/(double)RAND_MAX)

extern void llda_learn(document *data, double alpha, double beta, int nclass, int nlex, int dlenmax, int maxiter, double **phi, double **theta, int **n_mz, int **n_zw, FILE *likp);
#endif
