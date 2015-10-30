


int julia(const double *x, int xres, const double *y, int yres, const double *c, int flag, int maxIterations, int *iterations, double xgap, double ygap, int i, int j);
void getParams(char **argv,  int *flag, double *c, double *x, double *y, int *width, int *height, int *maxiter, char **image, char **stats);
void iterations2color(int width, int height, const int *iterations, int max_iterations, int *image);

void saveBMP(char* filename, int* result, int width, int height);
void saveSTAT(char* filename, int* result, double * times, int p);
