/*
    likelihood.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static double log_multi_beta_vector(int *vec, int length, double padding){
    double *new_vec;
    int i;
    double left;
    double right;
    
    if((new_vec = calloc(length,sizeof(double))) == NULL){
        fprintf(stderr,"lda_likelihood:: cannot allocate new_vec.\n");
        exit(1);
    }
    
    for(i = 0;i < length;i++){
        new_vec[i] = (double)vec[i] + padding;
    }
    
    left = 0.0;
    for(i = 0;i < length;i++){
        left += lgamma(new_vec[i]);
    }
    right = 0.0;
    for(i = 0;i < length;i++){
        right += new_vec[i];
    }
    right = lgamma(right);
    
    return (left - right);
}

static double log_multi_beta_scalar(double val, int K){
    return ((double)K * lgamma(val) - lgamma((double)K * val));
}

double loglikelihood(int **n_mz, int **n_zw, int nclass, int nlex, int ndocs, double alpha, double beta){
    double lik = 0.0;
    int k;
    
    for(k = 0;k < nclass;k++){
        lik += log_multi_beta_vector(n_zw[k], nlex, beta);
        lik -= log_multi_beta_scalar(beta, nlex);
    }
    
    for(k = 0;k < ndocs;k++){
        lik += log_multi_beta_vector(n_mz[k], nclass, alpha);
        lik -= log_multi_beta_scalar(alpha, nclass);
    }
    
    return lik;
}
