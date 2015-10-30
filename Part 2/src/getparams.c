
#include "getparams.h"

#define STRLENTH 256
#define NUMBER_OF_LINES 14

//Choose ONE only!
//#define gmp
//#define standardC // use standard C defined variables. BROKEN

void getParams(char* params, char **argv)
//#endif
{
     FILE *file;
     int i;
     file = fopen(argv[1], "r");
     char *tempchar = (char*)malloc(sizeof(char) * STRLENTH);
     printf("Opening: %s\n",argv[1]);
     
     for(i=0;i<=NUMBER_OF_LINES;i++)
     {
       fscanf(file, "%s", tempchar);
       memcpy(&params[i*STRLENTH],tempchar,STRLENTH);
       printf("%s\n",&params[i*STRLENTH]);
     }
     fclose(file);
     free(tempchar);
}

void xtractParams(int* flag, mpf_t* cr,mpf_t* ci, mpf_t* x,mpf_t* y, mpf_t* init_xr, mpf_t* init_yr, mpf_t* final_xr, mpf_t* final_yr,
               int* width, int* height, int* maxiter, mpf_t* zoom_rate, char *image, char *image_type, char *params, int rank)
{
   // int error;
  //mpz_init_set_str (*flag, &params[0*STRLENTH], 10);
  *flag = strtol(&params[0*STRLENTH], NULL,0);
  mpf_init_set_str (*cr, &params[1*STRLENTH], 10);
  mpf_init_set_str (*ci, &params[2*STRLENTH], 10);
  mpf_init_set_str (*x, &params[3*STRLENTH], 10);
  mpf_init_set_str (*y, &params[4*STRLENTH], 10);
  mpf_init_set_str (*init_xr, &params[5*STRLENTH], 10);
  mpf_init_set_str (*init_yr, &params[6*STRLENTH], 10);
 // printf ("Error: %i\n", error);
  mpf_init_set_str (*final_xr, &params[7*STRLENTH], 10);
  mpf_init_set_str (*final_yr, &params[8*STRLENTH], 10);

  *width = strtol(&params[9*STRLENTH], NULL,0);
  *height = strtol(&params[10*STRLENTH], NULL,0);
  *maxiter = strtol(&params[11*STRLENTH], NULL,0);
  mpf_init_set_str (*zoom_rate, &params[12*STRLENTH], 10);
  //*image = params[13*STRLENTH];
  //*image_type = params[14*STRLENTH];
 // mpz_init_set_str (*zoom_rate, &params[12*STRLENTH], 10);
  memcpy(image,&params[13*STRLENTH],STRLENTH);
  memcpy(image_type, &params[14*STRLENTH],STRLENTH);
}
