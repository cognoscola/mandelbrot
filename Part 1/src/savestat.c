#include<stdio.h>
#include<stdlib.h>

void saveSTAT(char* filename, int* data, double* times, int p){
   int i;
   FILE *f;  
   f = fopen(filename,"wb");
   char names[] = { "Process   Time          Max Iteration\n"};
   char space[] = { "        " };
   char newline[] = {"\n"};

   fputs(names, f);
   for(i=0; i<p; i++)
   {
        fprintf(f, "%d ", data[i*2]);
        fputs(space, f);
	fprintf(f, "%f", times[i]);
        fputs(space, f);
        fprintf(f, "%d", data[i*2+1]);
	fputs(newline, f);
   }
   fclose(f);
	
}
