#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "parallel-julia.h"
#include "julia.h"
#include "mpi.h"
#include <unistd.h>


int parallelJulia(const double *x, int xres, const double *y, int yres, const double *c, int flag, int maxIterations, int my_rank, int p, MPI_Comm com, char * image, char * statfile)
{
   int k=0, i, j;
   int local_t;
   int *iterations;
   int *Fiterations; 

   int *stats; 
   int *mystat; 
   double *times; 
   double *mytime;

   double starttime;
   double endtime;

   int evenly = 0; 
   int starting_pixel, ending_pixel;
   int pos;
   double xgap, ygap;
   int maxCount=0;

  

  /* Give each task an equal number of pixels to process on */ 
  k =  xres*yres; 
  if((k%p)==0) { 
	local_t = k/p;
        k = 0;
        evenly = 0;
  } else {
	local_t = (k - k%p)/p;
        k = k%p;   
        evenly = 1; 
 
  }
  local_t  = (my_rank<k) ? local_t+1:local_t;

  /*Have each seperate process allocate its own chunk of memory to work on  */	
  iterations = (int*)malloc( sizeof(int) * local_t );
  mytime =  malloc( sizeof(double) ); 
  mystat  = (int*)malloc( sizeof(int) * 2);
  *mystat = my_rank;

  /* each process should calculate the position of the pixels they are working on */  
   starting_pixel = (my_rank<k && evenly==1) ? ( my_rank * (local_t-1) + my_rank):(my_rank * local_t + k); 
   ending_pixel = starting_pixel + local_t-1;
    
   xgap = (x[1] - x[0]) / xres;
   ygap = (y[1] - y[0]) / yres;
  
   int* pointer_index = iterations;
   
   //start timer
   starttime = MPI_Wtime();
   for(pos =  starting_pixel; pos <= ending_pixel; pos++)
   {   
	int max;
        i = pos%xres;
        j = (pos - i)/xres;      
        max = julia(x, xres, y, yres, c, flag, maxIterations, pointer_index, xgap, ygap, i, j);  
        if(max > maxCount) {maxCount = max; }
        pointer_index++;                   
   } 
   //end timer
   endtime = MPI_Wtime();
   *mytime = endtime-starttime;
   mystat[1] = maxCount;
 
   /* Send back the pixel data to process 0.  */
    if(my_rank == 0)
    {
        Fiterations = (int*)malloc( sizeof(int) * xres * yres );
 	assert(Fiterations); //check for error and abort of error is present 
    }

 /* Send back stat data to process 0.  */
    if(my_rank == 0)
    {
        //allocate space for int data
        stats = (int*)malloc( sizeof(int) * p * 2  );
 	assert(stats); //check for error and abort of error is present 
        //allocate space for double data
        times = (double*)malloc( sizeof(double) * p);
    }
 
   //Collect Pixel Data
   MPI_Gather(iterations,local_t, MPI_INT, Fiterations, local_t, MPI_INT, 0, com); 
   //Collect Stat->int Data
   MPI_Gather(mystat,2, MPI_INT, stats, 2, MPI_INT, 0, com); 
   //Collect Stat->double Data
   MPI_Gather(mytime, 1, MPI_DOUBLE, times, 1, MPI_DOUBLE, 0, com); 
 

   /* save our picture for the viewer */
   if(my_rank==0)
   {
      saveBMP(image, Fiterations, xres, yres);
      free(Fiterations);
   }
   
   if(my_rank==0)
   {
       saveSTAT(statfile, stats, times, p);
       free(stats);
	
   }

   free(iterations);
   free(mystat);
   free(mytime);
   return 0;
} 

