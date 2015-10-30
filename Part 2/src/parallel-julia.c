#include "parallel-julia.h"

#define STATSIZE 100
#define STRLENTH 256


void getStats(double elapsedTime, int numcores, long long *maxiter, char *timestats)
{
  char *timestat;
  timestat = (char*)malloc(STATSIZE*sizeof(char));
  memset(timestat,'\0',STATSIZE);
  sprintf(timestat, "%d %f %lld\n",numcores, elapsedTime, *maxiter);
  memcpy( timestats, timestat, STATSIZE*sizeof(char) );
  free(timestat);
}

long long parallelJulia(mpf_t *x, int xres, mpf_t *y, int yres, mpf_t *c, int flag, int* maxIterations, int my_rank, int p, MPI_Comm com, char * image)//, char * statfile)
{
    //To do : remove the overhead from gmp and use integer as much as possible.

       //Initalize Variables
       int i, j,k, evenly, local_t, starting_pixel, ending_pixel, pos, maxCount, max;
       int *iterations;
       int *Fiterations;
       double starttime, endtime, total_time, max_time;
       mpf_set_default_prec(128);
       mpf_t ygap_f, xgap_f;
       mpf_init(ygap_f);
       mpf_init(xgap_f);

      //start timer
       starttime = MPI_Wtime();

      /* Give each task an equal number of pixels to process on */
      k = xres*yres;


      if(k % p == 0)
      {
            local_t = k/p;
            k = 0;
            evenly = 0;
      }
      else
      {
            local_t = (k - k%p)/p;
            k = k%p;
            evenly = 1;

      }
      local_t  = (my_rank<k) ? local_t+1:local_t;

      /*Have each seperate process allocate its own chunk of memory to work on  */
      iterations = (int*)malloc( sizeof(int) * local_t);


      //Starting and Ending Pixel Calculation
       starting_pixel = (my_rank<k && evenly==1) ? ( my_rank * (local_t-1) + my_rank):(my_rank * local_t + k);
       ending_pixel = starting_pixel + local_t-1;
       if (my_rank == 0)
       {
               ending_pixel-= 1;
       }

       //xgap = (x1 - x0) / xres
       //ygap = (y1 - y0) / yres
       mpf_set(xgap_f, x[1]);
       mpf_sub(xgap_f, xgap_f, x[0]);
       mpf_div_ui(xgap_f, xgap_f, xres);
       mpf_set(ygap_f, y[1]);
       mpf_sub(ygap_f, ygap_f, y[0]);
       mpf_div_ui(ygap_f, ygap_f, yres);

        starttime = MPI_Wtime();
#ifdef _OPENMP

        //changed it to two for testing on my local
        //computer. Change it to 4 for using in BGQ
        int thread_count = 2;

        if(my_rank==0)
        {
           // printf("Using %d Threads....\r",thread_count);
        }

#pragma omp parallel num_threads(thread_count) default(none) private(pos,i,j) \
        shared(iterations, ygap_f,xgap_f, maxIterations,flag,c,y,x,starting_pixel, ending_pixel,xres,maxCount,max,local_t)
       {
           //allocate variables thread variables
           int thread_rank = omp_get_thread_num();
           int pthreads = omp_get_num_threads();
           int T;
           int* pointer;
           int even;
           int thread_local_t;

           //devide local_t by the number of threads evenly.
           //note: it is possible to pass these value instead of
           //computing it over and over again everytime a
           //thread is created.
           T = local_t;
           if(T % pthreads== 0)  //divides evenly
           {
               thread_local_t = T/pthreads;
               T = 0;
               even = 0;
           }
           else
           {
               thread_local_t = (T - T%pthreads)/pthreads;
               T = T % pthreads;
               even = 1;
           }
           thread_local_t = (thread_rank<T) ? thread_local_t+1:thread_local_t;

           pointer = (int *)iterations;
           pointer += (thread_rank < T && even==1) ? (thread_rank * (thread_local_t - 1) + thread_rank):(thread_rank * thread_local_t + T);
#          pragma omp for
           for(pos =  starting_pixel; pos <= ending_pixel; pos++)
           {
                i = pos%xres;
                j = (pos - i)/xres;
                max = julia(x, y, c, flag, *maxIterations, pointer, xgap_f, ygap_f, i, j);
                if(max > maxCount) {maxCount = max;}
                pointer++;
           }

       }

#else

        if(my_rank==0)
        {
            printf("Using 1 Thread\r");
        }

          int* pointer_index = iterations;
          for(pos =  starting_pixel; pos <= ending_pixel; pos++)
          {
               i = pos%xres;
               j = (pos - i)/xres;
               max = julia(x, y, c, flag, *maxIterations, pointer_index, xgap_f, ygap_f, i, j);
               if(max > maxCount) {maxCount = max;}
               pointer_index++;
          }
#endif

       //end timer
       endtime = MPI_Wtime();

       /* Send back the pixel data to process 0.  */
        if(my_rank == 0) //Keep this condition here. There is no point in having ALL processes allocate
                         //width x height amount of pixels. Thats alot of wasted memory! The compiler warns about it
                         //since -Wall is one of compile options.
        {
            Fiterations = (int*)malloc( sizeof(int) * xres * yres);
            assert(Fiterations); //check for error and abort of error is present
        }

       //MPI_Gather(iterations,(int)mpz_get_ui(local_t_z), MPI_UNSIGNED_LONG, Fiterations, (int)mpz_get_ui(local_t_z), MPI_UNSIGNED_LONG, 0, com);
       MPI_Gather(iterations,local_t, MPI_INT, Fiterations, local_t, MPI_INT, 0, com);
       // MPI_Reduce(&processTotalIterations, &total, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);


       total_time = endtime-starttime;
       MPI_Reduce(&total_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
       /* save our picture for the viewer */
       if(my_rank==0)
       {
          //char* timestats = (char*)malloc( sizeof(char) * STATSIZE);
          saveBMP(image, Fiterations, xres, yres);
          free(Fiterations);
          //getStats(max_time,p,timestats);
          //printf(timestats);
       }

      // printf("Rank %d finished! Time = %f\n", my_rank, total_time);
       mpf_clear(ygap_f);
       mpf_clear(xgap_f);
       free(iterations);
       return 0;
} 

