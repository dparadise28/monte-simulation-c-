// This file declares various functions that are useful over the course
// of the semester.  It must be "included" above the main program.


// These are some standard C function libraries.
#include <stdlib.h>  // "standard library"
#include <math.h>    // various math functions, such as exp()
#include <stdio.h>   // various "input/output" functions
#include <time.h>    // functions for timing computations


// These functions are found in "4135FunctionLibrary.h".
double MTUniform (unsigned int);
double LCGUniform (unsigned int);
double MWCUniform (unsigned int);
double LCG64Uniform (int unsigned);
void   Pause ();
double Histogram (double, double, double, int, int);
double DiscreteHistogram (int, int, int, int);
void   NormalHistogram (double, int, int);
void   ExponentialHistogram (double, int, int);
void   UniformHistogram (double, int, int);
int    Equal (double, double, double);
double **Array (int, int);
double PolarNormal ();
double PsiTS (double);
double Psi (double);
double PsiInv (double);
void   CorrelatedNormals (double, double *);



