/*
    feature.h
    a header file for feature matrix
*/
#ifndef FEATURE_H
#define FEATURE_H
#include <stdio.h>

typedef struct {
    int len;
    int *id;
    int *cnt;
    int labellen;
    int *labelid;
} document;

extern document *feature_matrix(char *filename, int *maxid, int *maxfeat,int *ndoc);
extern int assign_labels(char *filename, document *data, int *max_labelid);
extern void free_feature_matrix(document *matrix);

#endif
