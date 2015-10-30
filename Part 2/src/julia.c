#include <assert.h>
#include <math.h>
#include "julia.h"

//To do:
// Change Count and functions to use count as int             - done
// change maxiteration to int and use functions as as int.    - done

int julia(mpf_t *x, mpf_t *y, mpf_t *c,
          int flag, int maxIterations, int *iterations, mpf_t xgap, mpf_t ygap, int i, int j)
{
  int count =0;
  mpf_t xi, yi,  savex, savex2, savey2, savey, radius, maxRadius, data_holder_A, data_holder_B;
  mpf_set_default_prec(128);
  mpf_init(xi);
  mpf_init(yi);
  mpf_init(savex);
  mpf_init(savey);
  mpf_init(savex2);
  mpf_init(savey2);
  mpf_init(radius);
  mpf_init(maxRadius);
  mpf_init(data_holder_A);
  mpf_init(data_holder_B);

  /* pixel to coordinates */
	//  xi = x[0] + i * xgap;
	//  yi = y[0] + j * ygap;
  mpf_set_si(xi, i);
  mpf_set_si(yi, j);
  mpf_mul(xi, xi, xgap);
  mpf_mul(yi, yi, ygap);
  mpf_add(xi, xi, x[0]);
  mpf_add(yi, yi, y[0]);
  
   /* initial value for the iteration */
   //	  savex = flag*c[0] + (1-flag)*xi;
   //	  savey = flag*c[1] + (1-flag)*yi;

  //savex = xi
   mpf_set_ui(data_holder_B, (unsigned long)flag); // flag -> int
   mpf_neg(data_holder_B, data_holder_B); // flagint -> -flagint
   mpf_add_ui(data_holder_B, data_holder_B, 1); // -flagint + 1


   mpf_set_ui(data_holder_A, (unsigned long)flag); // flag -> flagint
   mpf_mul(data_holder_A, data_holder_A, c[0]); //creal*flagint

   //savex = xi* (1- flag);
   //savex = xi (1 - flag) + flag*c[0];
   mpf_set(savex, xi); //savex = xi
   mpf_mul(savex, savex, data_holder_B); //savex*(1-flagint)
   mpf_add(savex, savex, data_holder_A); //xi*(1-flagint) + creal*flagint

   mpf_set_ui(data_holder_A, (unsigned long)flag);
   mpf_mul(data_holder_A, data_holder_A, c[1]);
   
   mpf_set(savey, yi);
   mpf_mul(savey,savey, data_holder_B);
   mpf_add(savey, savey, data_holder_A);

   mpf_set_ui(maxRadius, 4.0);
          while ( mpf_cmp(radius, maxRadius) <= 0 && count < maxIterations)
	    {
	     mpf_set(savex2, xi);
		 mpf_set(savey2, yi);
		 mpf_mul(savey2, savey2, savey2); //yi -> yi^2

		 //xi = xi * xi - yi * yi + savex; // real part x^2 +c 
		 mpf_mul(xi, xi, xi);
		 mpf_add(xi, xi, savex);
		 mpf_sub(xi, xi, savey2);

		  // yi = 2.0f * savex2 * yi + savey; // imaginary part x*2 +c
		  //mpf_set(yi, savey2);
		  mpf_mul(yi, yi, savex2);
		  mpf_mul_ui(yi,yi, 2.0);
		  mpf_add(yi,yi,savey);
		  
		  //radius = xi * xi + yi * yi;      //adding them up. 
		  mpf_set(radius, xi);
		  mpf_mul(radius, radius, xi);
		  mpf_add(radius, radius, savey2);

                  count++;
	    }
	  
          if( count > maxIterations)
	  {
                 count = maxIterations;
	  }
	    
          int *p = iterations;

	  /* If radius <= 4.0, we have hit maxIterations. The point is
	     likely in the set. */
	  if ( mpf_cmp(radius, maxRadius) <= 0)
	    {
	      *p = 0;
	    }
	  else
	    /* radius > 4.0. The point tends to infinity. We recognize
	       this at iteration number count */
	    {
              *p = count;
	    }
	
  mpf_clear(xi);
  mpf_clear(yi);
  mpf_clear(savex);
  mpf_clear(savey);
  mpf_clear(savex2);
  mpf_clear(savey2);
  mpf_clear(radius);
  mpf_clear(maxRadius);
  mpf_clear(data_holder_A);
  mpf_clear(data_holder_B);

  return count;
}

