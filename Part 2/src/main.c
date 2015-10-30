#include "parallel-julia.h"
#include "getparams.h"

/*
TO DO:
    - Calculate Parameters to pass to Parallel Julia          -In Progress
    - Calculate Time spent by each process                    -DONE
    - Implement 4 Threads PER core using OpenMP               -DONE
    - Implement saving frames into a folder /w correct names  -In Progress
    - Make Threads more efficient by passing (instead
       of computing) useful values at initialization          - Waiting
    - optimize parallel-julia/julia even more.                - Waiting
*/

/* enable if you want output info */
//#define debug

//Definitions for tags
#define ROW_ITER_HEIGHT 1 //We want each process to handle 1 row at a time? 
#define KEEP_WORKING 1
#define STOP_WORKING 2
#define STATSIZE 100
#define STRLENTH 256

#define FRAMECNT 2000


// void getStats(double elapsedTime, int numcores, mpz_t *maxiter, char *timestats)
// {
//   char *timestat;
//   timestat = (char*)malloc(STATSIZE*sizeof(char));
//   memset(timestat,'\0',STATSIZE);
//   sprintf(timestat, "%d %f %ld\n",numcores, elapsedTime, mpz_get_ui(*maxiter));
//   memcpy( timestats, timestat, STATSIZE*sizeof(char) );
//   free(timestat);
// }
// void writeStats(char *timestats)
// {
//     printf(timestats);
// 
// }

int main(int argc, char *argv[])
{
  int my_rank, p;
  long long total;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

// MPI_Aint	offsets[11];
  int error_code;
  char *params;
  char *image;
  char *image_type;
//   char *timestats;
//   double starttime,endtime;

  int *flag;
  int *width;
  int *height;
  int *maxiter;

  int framepos=0;


  mpf_t *cc;
  mpf_t *xrange;
  mpf_t *yrange;
  mpf_t *cr;
  mpf_t *ci;
  mpf_t *x;
  mpf_t *y;
  mpf_t *init_xr;
  mpf_t *init_yr;
  mpf_t *final_xr;
  mpf_t *final_yr;
  mpf_t *zoom_rate;
  mpf_t *xd;
  mpf_t *yd;

  
  //Change for Int
  flag    = (int*)malloc(sizeof(int));
  width   = (int*)malloc(sizeof(int));
  height  = (int*)malloc(sizeof(int));
  maxiter = (int*)malloc(sizeof(int));

  //zoom_rate = (double *)malloc(sizeof(double));

  cr         = (mpf_t*)malloc(sizeof(mpf_t));
  ci         = (mpf_t*)malloc(sizeof(mpf_t));
  x          = (mpf_t*)malloc(sizeof(mpf_t));
  y          = (mpf_t*)malloc(sizeof(mpf_t));
  init_xr    = (mpf_t*)malloc(sizeof(mpf_t));  //
  init_yr    = (mpf_t*)malloc(sizeof(mpf_t));  //
  final_xr   = (mpf_t*)malloc(sizeof(mpf_t));  //
  final_yr   = (mpf_t*)malloc(sizeof(mpf_t));  //
  xd         = (mpf_t*)malloc(sizeof(mpf_t));
  yd         = (mpf_t*)malloc(sizeof(mpf_t));
  zoom_rate  = (mpf_t*)malloc(sizeof(mpf_t));


  image_type = (char*)malloc(sizeof(char)*STRLENTH);
  image      = (char*)malloc(sizeof(char)*STRLENTH);
 // filename   = (char*)malloc(sizeof(char)*STRLENTH);
  params     = (char*)malloc(sizeof(char)*STRLENTH*15);
  xrange     = (mpf_t*)malloc(sizeof(mpf_t)*2);
  yrange     = (mpf_t*)malloc(sizeof(mpf_t)*2);
  cc 	     = (mpf_t*)malloc(sizeof(mpf_t)*2);
  mpf_init(xrange[0]);
  mpf_init(xrange[1]);
  mpf_init(yrange[0]);
  mpf_init(yrange[1]);
  mpf_init(*xd);
  mpf_init(*yd);
  mpf_init(cc[0]);
  mpf_init(cc[1]);


  if (my_rank == 0)
  {     
      if(argc >=2){
              getParams(params, argv);
           } else {
              printf("Indicate the file name containing the parameters. kthxbye!\n");
           }
  }
  //Receive data
  error_code = MPI_Bcast(params, STRLENTH*15, MPI_CHAR, 0, MPI_COMM_WORLD);

#ifdef debug
  if (my_rank == 2)
  {
     printf("RANK = %d As chars-------------\n",my_rank);
      //See data as Char
      int i;
      for(i=0;i<=14;i++)
      {       
        printf("%s\n",&params[i*256]);
      }
  }

#endif

  MPI_Barrier(MPI_COMM_WORLD);
  xtractParams(flag, cr, ci, x, y, init_xr, init_yr,final_xr,final_yr, width, height, maxiter, zoom_rate, image,image_type, params, my_rank);

#ifdef debug
 if (my_rank == 2)
  {

      printf("RANK = %d  AS datatypes-------------\n",my_rank);
      //See data as their types
      printf("Flag: %d \n", *flag);
      gmp_printf ("cr: %.Ff\n", *cr);
      gmp_printf ("ci:%.Ff\n", *ci);
      gmp_printf ("x: %.Ff\n", *x);
      gmp_printf ("y: %.Ff\n", *y);
      gmp_printf ("init_xr: %.Ff\n", *init_xr);
      gmp_printf ("init_yr: %.Ff\n", *init_yr);
      gmp_printf ("final_xr: %.Ff\n", *final_xr);
      gmp_printf ("final_yr: %.Ff\n", *final_yr);
      printf("Width: %d \n", *width);
      printf("Height: %d \n", *height);
      printf("Maxiter: %d \n", *maxiter);
      printf("Zoom: %f \n", *zoom_rate);
      printf("file: %s\n",image);
      printf("Type: %s\n",image_type);
  }
#endif

  mpf_mul_ui(*xd,*final_xr,2); //what is this for?
  mpf_mul_ui(*yd,*final_yr,2); //Why is this here?

  if(framepos==0){
      if(my_rank ==0) printf("Creating Frames! Progress: \n\n");
  }

  for(framepos = 0; framepos < FRAMECNT;framepos++ )
  {
      char num[15];
      sprintf(num, "%d", framepos);
      char filename[256];
      strcpy(filename,image);
      strcat(filename,num);
      strcat(filename,image_type);

      //calculate progress!
      double numer = (double)framepos;
      double denum = (double)FRAMECNT;

      if(my_rank ==0) printf("Frame %d of %d ....%.1f%% \r", framepos+1, FRAMECNT,((numer/denum)*100));

      //coordinate calculations
      //start
      if(framepos ==0)
      {
         mpf_set(cc[0], *cr);
         mpf_set(cc[1], *ci);

         mpf_sub(xrange[0], *x, *init_xr);
         mpf_add(xrange[1], *x, *init_xr);
         mpf_sub(yrange[0], *y, *init_yr);
         mpf_add(yrange[1], *y, *init_yr);
         total = parallelJulia(xrange, *width, yrange, *height, cc, *flag, maxiter, my_rank, p, MPI_COMM_WORLD, filename);
      }
      else
      {
         mpf_div(*init_xr,*init_xr,*zoom_rate);
         mpf_div(*init_yr,*init_yr,*zoom_rate);
         mpf_sub(xrange[0], *x, *init_xr);
         mpf_add(xrange[1], *x, *init_xr);
         mpf_sub(yrange[0], *y, *init_yr);
         mpf_add(yrange[1], *y, *init_yr);
         total = parallelJulia(xrange, *width, yrange, *height, cc, *flag, maxiter, my_rank, p, MPI_COMM_WORLD, filename);
      }

      MPI_Barrier(MPI_COMM_WORLD);

  }

  if(my_rank ==0){printf("Done!");}

  free(flag);
  free(width);
  free(height);
  free(maxiter);
  free(cr);
  free(ci);
  free(x);
  free(y);
  free(init_yr);
  free(init_xr);
  free(final_yr);
  free(final_xr);
  free(xd);
  free(yd);

 // free(iterations);
  free(cc);
  free(xrange);
  free(yrange);
  free(image);
  free(image_type);
  free(zoom_rate);
  free(params);
//   free(timestats);

  MPI_Finalize();
  return 0;
}


