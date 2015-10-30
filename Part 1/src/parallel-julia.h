/*parallel-julia.h */
#include "mpi.h"
int parallelJulia(const double *x, int xres, const double *y, int yres, const double *c, int flag, int maxIterations,int my_rank, int p, MPI_Comm com, char *image, char * statfile);
