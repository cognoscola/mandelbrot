/*parallel-julia.h */
#ifndef __PARALLEL_JULIA
#define __PARALLEL_JULIA

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _OPENMP
#include <omp.h>
#endif


#include "julia.h"

long long parallelJulia(mpf_t *x, int xres, mpf_t *y, int yres, mpf_t *c, int flag, int* maxIterations, int my_rank, int p, MPI_Comm com, char * image);//, char * statfile)


#endif
