#ifndef __GET_PARAMS
#define __GET_PARAMS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

void getParams(char* params, char **argv);

void xtractParams(int* flag,
                  mpf_t* cr,mpf_t* ci,mpf_t* x,mpf_t* y,
                  mpf_t* init_xr, mpf_t* init_yr, mpf_t* final_xr, mpf_t* final_yr,
                  int* width, int* height, int* maxiter, mpf_t*zoom_rate,
                  char *image, char *image_type, char *params, int rank);

#endif
