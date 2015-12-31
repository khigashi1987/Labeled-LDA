/*
    learn.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "MT.h"
#include "learn.h"
#include "feature.h"
#include "imatrix.h"
#include "dmatrix.h"
#include "util.h"
#include "likelihood.h"

void llda_learn(document *data, double alpha, double beta, int nclass, int nlex, int dlenmax, int maxiter, double **phi, double **theta, int **n_mz, int **n_zw, FILE *likp){
    document *dp;
    int ndocs;
    int *n_m;
    int *n_z;
    int ***topics;
    int word_index;
    int word_num;
    double *left;
    double *right;
    double *p_z;
    double sum_p_z;
    double *cum_sum_p_z;
    double sampling;
    double lik;
    double **temp_phi;
    double **temp_theta;
    int z;
    int it;
    int m, w, i, k;


    // count data length
    for(dp = data, ndocs = 0;(dp->len) != -1;dp++, ndocs++)
        ;
    
    // initialize buffers
    if((n_m = calloc(ndocs,sizeof(int))) == NULL){
        fprintf(stderr,"llda_learn:: cannot allocate n_m.\n");
        return;
    }
    if((n_z = calloc(nclass,sizeof(int))) == NULL){
        fprintf(stderr,"llda_learn:: cannot allocate n_z.\n");
        return;
    }
    if((left = calloc(nclass,sizeof(double))) == NULL){
        fprintf(stderr,"llda_learn:: cannot allocate left.\n");
        return;
    }
    if((right = calloc(nclass,sizeof(double))) == NULL){
        fprintf(stderr,"llda_learn:: cannot allocate right.\n");
        return;
    }
    if((p_z = calloc(nclass,sizeof(double))) == NULL){
        fprintf(stderr,"llda_learn:: cannot allocate p_z.\n");
        return;
    }
    if((cum_sum_p_z = calloc((nclass+1),sizeof(double))) == NULL){
        fprintf(stderr,"llda_learn:: cannot allocate cum_sum_p_z.\n");
        return;
    }
    if((topics = calloc(ndocs,sizeof(int **))) == NULL){
        fprintf(stderr,"llda_learn:: cannot allocate topics.\n");
        return;
    }
    if((temp_phi = dmatrix(nlex, nclass)) == NULL){
        fprintf(stderr,"llda_learn:: cannot allocate temp_phi.\n");
        exit(1);
    }
    if((temp_theta = dmatrix(ndocs, nclass)) == NULL){
        fprintf(stderr,"llda_learn:: cannot allocate temp_theta.\n");
        exit(1);
    }
    
    printf("Number of documents          = %d\n",ndocs);
    printf("Number of words              = %d\n",nlex);
    printf("Number of latent classes     = %d\n",nclass);
    printf("Number of iteration          = %d\n",maxiter);
    
    
    // choose an arbitrary topic as first topic for word
    init_genrand(time(NULL));
    for(dp = data, m = 0;(dp->len) != -1;dp++, m++){
        if((topics[m] = calloc((dp->len), sizeof(int *))) == NULL){
            fprintf(stderr,"llda_learn:: cannot allocate topics[m].\n");
            return;
        }
        for(w = 0;w < (dp->len);w++){
            if((topics[m][w] = calloc((dp->cnt[w]), sizeof(int))) == NULL){
                fprintf(stderr,"llda_learn:: cannot allocate topics[m][w].\n");
                return;
            }
            word_index = dp->id[w];
            word_num = dp->cnt[w];
            for(i = 0;i < word_num;i++){
                z = genrand_int32() % nclass;
                n_mz[m][z] += 1;
                n_m[m] += 1;
                n_zw[z][word_index] += 1;
                n_z[z] += 1;
                topics[m][w][i] = z;
            }
        }
    }
    
    // learning main
    for(it = 0;it < maxiter;it++){
        printf("iteration %2d/%3d..\n", it + 1, maxiter);
        fflush(stdout);
        for (dp = data, m = 0; (dp->len) != -1; dp++, m++){
            int *class_flags;
            class_flags = (int *)calloc(nclass,sizeof(int));
            for(i = 0;i < dp->labellen;i++)
                class_flags[dp->labelid[i]] = 1;
            for(w = 0;w < (dp->len);w++){
                word_index = dp->id[w];
                word_num = dp->cnt[w];
                for(i = 0;i < word_num;i++){
                    z = topics[m][w][i];
                    n_mz[m][z] -= 1;
                    n_m[m] -= 1;
                    n_zw[z][word_index] -= 1;
                    n_z[z] -= 1;
                    
                    // compute conditional distribution p_z
                    // p_z left
                    for(k = 0;k < nclass;k++){
                        left[k] = (double)n_mz[m][k] + alpha;
                        left[k] /= ((double)n_m[m] + (double)nclass * alpha);
                    }
                    // p_z right
                    for(k = 0;k < nclass;k++){
                        right[k] = (double)n_zw[k][word_index] + beta;
                        right[k] /= ((double)n_z[k] + (double)nlex * beta);
                    }
                    // conditional distribution p_z
                    sum_p_z = 0.0;
                    for(k = 0;k < nclass;k++){
                        if(class_flags[k] == 1){
                            p_z[k] = left[k] * right[k];
                            sum_p_z += left[k] * right[k];
                        }else{
                            p_z[k] = 0.0;
                        }
                    }
                    for(k = 0;k < nclass;k++){
                        p_z[k] /= sum_p_z; // normalize to obtain probabilities
                    }
                    
                    // random sampling from p_z
                    cum_sum_p_z[0] = 0.0;
                    for(k = 0;k < nclass;k++){
                        cum_sum_p_z[k+1] = cum_sum_p_z[k] + p_z[k];
                    }
                    sampling = genrand_real3();
                    for(k = 0;k < nclass;k++){
                        if((sampling >= cum_sum_p_z[k]) && (sampling < cum_sum_p_z[k+1])){
                            z = k;
                            break;
                        }
                    }
                    
                    // update buffers
                    n_mz[m][z] += 1;
                    n_m[m] += 1;
                    n_zw[z][word_index] += 1;
                    n_z[z] += 1;
                    topics[m][w][i] = z;
                }
            }
        }
        // compute likelihood.
        lik = loglikelihood(n_mz, n_zw, nclass, nlex, ndocs, alpha, beta);
        printf("\tlikelihood ... %.8f\n",lik);
        fprintf(likp,"%.8f\n",lik);
    }
    
    // compute matrix phi ([nlex, nclass] matrix)
    for(w = 0;w < nlex;w++)
        for(k = 0;k < nclass;k++)
            temp_phi[w][k] = (double)n_zw[k][w] + beta;
    normalize_matrix_col(phi, temp_phi, nlex, nclass);
    
    // compute matrix theta ([ndocs, nclass])
    for(m = 0;m < ndocs;m++)
        for(k = 0;k < nclass;k++)
            temp_theta[m][k] = (double)n_mz[m][k] + alpha;
    normalize_matrix_row(theta, temp_theta, ndocs, nclass);
    
    
    free(n_m);
    free(n_z);
    free(left);
    free(right);
    free(p_z);
    free(cum_sum_p_z);
    
    for(dp = data, m = 0;(dp->len) != -1;dp++, m++){
        for(w = 0;w < (dp->len);w++){
            free(topics[m][w]);
        }
        free(topics[m]);
    }
    free(topics);
    free_dmatrix(temp_phi, nlex);
    free_dmatrix(temp_theta, ndocs);
    
    return;
}
