#include <assert.h>
#include <math.h> 
#include "julia.h"

int julia(const double *x, int xres, const double *y, int yres, const double *c, 
	  int flag, int maxIterations, int *iterations, double xgap, double ygap, int i, int j)
{
  int maxIterationCount = 0;  
  double xi, yi,  savex, savey, radius;
  int count;


	  /* pixel to coordinates */
	  xi = x[0] + i * xgap;
	  yi = y[0] + j * ygap;
	  
	  /* initial value for the iteration */
	  savex = flag*c[0] + (1-flag)*xi;
	  savey = flag*c[1] + (1-flag)*yi;
	  
	  radius = 0.0;
	  count = 0;
	  while ( radius <= 4.0 && count < maxIterations )
	    {
	     double savex2 = xi;
	      xi = xi * xi - yi * yi + savex; // real part x^2 +c 
	      yi = 2.0f * savex2 * yi + savey; // imaginnary part x*2 +c
	      radius = xi * xi + yi * yi;      //adding them up. 
	      count++;

	    }
	  
	  if(count > maxIterationCount )
	    maxIterationCount = count;
	  
	  int *p = iterations;

	  /* If radius <= 4.0, we have hit maxIterations. The point is
	     likely in the set. */
	  if (radius <= 4.0)
	    {
	      assert(count==maxIterations);
	      *p = 0;
	    }
	  else
	    /* radius > 4.0. The point tends to infinity. We recognize
	       this at iteration number count */
	    {
	      *p = count;
	    }
	
    
  
  return maxIterationCount;
}

