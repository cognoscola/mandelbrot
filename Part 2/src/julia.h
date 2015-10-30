#ifndef __JULIA_H
#define __JULIA_H

#include <gmp.h>
#include "mpi.h"


int julia(mpf_t *x, mpf_t *y, mpf_t *c, int flag, int maxIterations,int *iterations, mpf_t xgap, mpf_t ygap, int i, int j);
//void getParams(char** flag, char** cr,char** ci,char** x,char** y,char** xr,char** yr, char** width,
//               char** height, char** maxiter,char **image,char **argv);

void iterations2color(int width, int height, const int *iterations, int max_iterations, int *image);

void saveBMP(char* filename, int* result, int width, int height);
void saveSTAT(char* filename, int* result, double * times, int p);


#endif
