/*
    feature.c
    an implementation of feature matrix
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "feature.h"
#define BUFFSIZE 65536
static int n_fields(char *line);
static int n_fields_label(char *line);
static int file_lines(FILE *fp);
static int isspaces(char *s);

document *feature_matrix(char *file_name, int *maxid, int *maxlen, int *ndoc){
    document *d;
    int n, m;
    FILE *fp;
    char line[BUFFSIZE];
    *maxid = -1;
    *maxlen = 0;
    
    if((fp = fopen(file_name,"r")) == NULL)
        return NULL;
    m = file_lines(fp);
    
    if((d = (document *)calloc(m+1, sizeof(document))) == NULL)
        return NULL;
    d[m].len = -1;
    
    n = 0;
    while(fgets(line, sizeof(line), fp)){
        int i, len;
        char *cp, *sp, *lp = line;
        
        if(isspaces(line))
            continue;
        
        len = n_fields(line);
        if(len > *maxlen)
            *maxlen = len;
        if(!(len > 0)){
            fprintf(stderr, "feature_matrix: suspicious line:\n%s", line);
            exit(1);
        }
        
        d[n].id = (int *)calloc(len, sizeof(int));
        d[n].cnt = (int *)calloc(len, sizeof(int));
        d[n].len = len;
        if((d[n].id == NULL) || (d[n].cnt == NULL))
            return NULL;
        
        i = 0;
        while(*lp){
            int id;
            double cnt;
            if((cp = strchr(lp, ':')) == NULL)
                break;
            if((sp = strpbrk(cp+1, " \t\n")) == NULL)
                break;
            *cp = '\0';
            *sp = '\0';
            id = atoi(lp) - 1;
            cnt = atoi(cp + 1);
            if(id >= *maxid)
                *maxid = id + 1;
            d[n].id[i] = id;
            d[n].cnt[i] = cnt;
            lp = sp + 1;
            i++;
        }
        n++;
    }
    fclose(fp);
    *ndoc = n;
    return d;
}

int assign_labels(char *file_name, document *data, int *max_labelid){
    int n,m;
    FILE *fp;
    char line[BUFFSIZE];
    *max_labelid = -1;
    
    if((fp = fopen(file_name,"r")) == NULL)
        return 0;
    
    m = file_lines(fp);
    n = 0;
    while(fgets(line, sizeof(line), fp)){
        int i, len;
        char *sp, *lp = line;
        
        if(isspaces(line)){
            fprintf(stderr, "feature_matrix: suspicious line:\n%s", line);
            exit(1);
        }
        len = n_fields_label(line);
        if(!(len > 0)){
            fprintf(stderr, "feature_matrix: suspicious line:\n%s", line);
            exit(1);
        }
        
        data[n].labelid = (int *)calloc(len,sizeof(int));
        data[n].labellen = len;
        if(data[n].labelid == NULL)
            return 0;
        
        i = 0;
        while(*lp){
            int id;
            if(isspace(*lp)){
                lp++;
                continue;
            }
            if((sp = strpbrk(lp, " \t\n")) == NULL)
                break;
            *sp = '\0';
            id = atoi(lp) - 1;
            if(id >= *max_labelid)
                *max_labelid = id + 1;
            data[n].labelid[i] = id;
            lp = sp + 1;
            i++;
        }
        n++;
    }
    fclose(fp);
    return 1;
}

void free_feature_matrix(document *matrix){
    document *dp;
    for(dp = matrix; (dp->len) != -1; dp++){
        free(dp->id);
        free(dp->cnt);
        free(dp->labelid);
    }
    free(matrix);
}

static int file_lines(FILE *fp){
    int n = 0;
    char buff[BUFFSIZE];
    
    while(fgets(buff,sizeof(buff),fp)){
        if(!isspaces(buff))
            n++;
    }
    rewind(fp);
    
    return n;
}

static int n_fields(char *line){
    int n = 0;
    char *cp, *sp, *lp = line;
    
    while(*lp){
        if((cp = strchr(lp, ':')) == NULL)
            break;
        if((sp = strpbrk(cp+1, " \t\n")) == NULL)
            break;
        lp = sp + 1;
        n++;
    }
    
    return n;
}

static int n_fields_label(char *line){
    int n = 0;
    char *sp, *lp = line;
    
    while(*lp){
        if(isspace(*lp)){
            lp++;
            continue;
        }
        if((sp = strpbrk(lp, " \t\n")) == NULL)
            break;
        lp = sp + 1;
        n++;
    }
    
    return n;
}

static int isspaces(char *s){
    char *c = s;
    while(*c){
        if(!isspace(*c))
            return 0;
        c++;
    }
    
    return 1;
}
