#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "julia.h"
#include "parallel-julia.h"

//#define debug1
//#define debug2

int main(int argc, char *argv[])
{
  /* define variables for Julia set */
  int width, height, maxiter, flag;
  double x[2], y[2], c[2];
  char *image, *stats; 

  /* variables for MPI */
  int my_rank, p; 

  MPI_Status status;
  MPI_Comm com = MPI_COMM_WORLD;
  MPI_Init(&argc, &argv); 
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p); 
 
  /*If Process 0, gather input and send to other processes */ 
  if(my_rank == 0) {
	getParams(argv, &flag, c, x, y, &width, &height, &maxiter, &image, &stats);       
  }

  /* Broad Cast the Data everywhere */ 
  MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(x, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(y, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(c, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&maxiter, 1, MPI_INT, 0, MPI_COMM_WORLD);

#ifdef debug1
printf("RANK:%d: FLAG=%d C0=%f,C1=%f,X0=%f,X1=%f,Y0=%f,Y1=%f,W=%i,H=%i,maxitem=%i,image=%d,stats=%d\n", my_rank, flag,c[0],c[1],x[0],x[1],y[0],y[1],width, height,maxiter,image,stats);
#endif

  /* compute set */
  parallelJulia(x, width, y, height, c, flag, maxiter, my_rank, p,com, image, stats);
  
printf("RANK %d has reached finalize!\n", my_rank);
  MPI_Finalize();
  return 0;
}




