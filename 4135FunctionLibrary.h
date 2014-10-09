// This file contains various functions that are useful over the course
//   of the semester.  They must be declared above the main program and this
//   file must be "included" following the main program.


// First are some random number generators.

////////////////////////////////////////////////////////////////////////////////
// MERSENNE TWISTER
// By M. Matsumoto and T. Nishimura (1998).
// "Mersenne Twister: a 623-dimensionally equidistributed uniform pseudo-random
//   number generator".
// ACM Transactions of Modeling and Computer Simulation 8(1):3-30.
// Any coding errors introduced are my own (C.D. Howard).

// An unsigned integer is represented by 32 bits in base 2.  The largest unsigned integer is:
// 2^32 - 1 = 4,294,967,295 (base 10);
//          = ffffffff (base 16);
//          = 1111 1111 1111 1111 1111 1111 1111 1111 (base 2).
// The digits in hexadecimal (base 16) are 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, a, b, c, d, e, f.

double MTUniform (unsigned int seed) {

   static unsigned int X[624], m[2], seeded = 0;
   static int i0, next[624], shift[624];
   unsigned int k, N;

   // Intitialize values the first time this function is called.
   if (!seeded) {
      if (!seed) {
         printf ("MTUniform must be seeded with a postive integer.\n");
         Pause ();
      }
      X[0] = seed;
      for (k = 0; k < 624; k++) {
         if (k) X[k] = 22695477 * X[k-1] + 1;   // Seed with your favorite LCG.
         if (k < 623) {
            next[k] = k + 1;
         } else {
            next[k] = 0;
         }
         // Here's the twist.
         if (k < 227) {
            shift[k] = 397;
         } else {
            shift[k] = -227;
         }
      }
      m[0] = 0; m[1] = 0x9908b0df;
      i0 = 624;
      seeded = 1;
      return (0);
   }

   // Generate the next 624 32-bit integers as needed.
   if (i0 == 624) {
      for (k = 0; k < 624; k++) {
         N = (X[k] & 0x80000000) | (X[next[k]] & 0x7fffffff);
         X[k] = X[k+shift[k]] ^ (N >> 1) ^ m[N & 1];
      }
      // Reset the counter.
      i0 = 0;
   }

   // Grab the next number from the list.
   N = X[i0];

   // Increment the counter.
   i0 ++;

   // Temper.
   N ^= (N >> 11);
   N ^= (N << 7) & 0x9d2c5680;
   N ^= (N << 15) & 0xefc60000;
   N ^= (N >> 18);

   // Now 0 <= N <= 4,294,967,295; scale it to be on the interval (0,1).
   return ( (N + 0.5) / 4294967296.0 );

}




////////////////////////////////////////////////////////////////////////////////
////// A typical 32 bit Linear Congruential Generator.

double LCGUniform (unsigned int seed) {

   // Static variables retain their values between function calls.
   static unsigned int N=1, a = 22695477, c = 1, seeded = 0;

   // Seed the LCG function the first time called...
   if (!seeded) {
      if (!seed) {
         printf ("LCGUniform must be seeded with a postive integer.\n");
         Pause ();
      }
      N = seed;
      seeded = 1;
      return (0);
   }

   // ...otherwise, generate the next number in the sequence.
   //    Here mod 2^32 is automatic since an unsigned integer is represented by 32 bits.
   N = a * N + c;

   // Now 0 <= N <= 4,294,967,295; scale it to be on the interval (0,1).
   return ( (N + 0.5) / 4294967296.0 );

}


////////////////////////////////////////////////////////////////////////////////
////// A Typical Multiply With Carry Generator.

// This function generates random numbers uniformly on [0,1].
// The numbers generated are of the form (n + 0.5) / 2^32, where 0 <= n <= 4,294,967,295 = 2^32 - 1.

// It uses the multiply-with-carry algorithm with m = 2^32, a = 4,294,967,118, and initial c = 1.
// Its period is 9,223,371,654,602,686,463 which is approximately 10^19.

// An unsigned integer is represented by 32 bits in base 2.  The largest unsigned integer is:
// 2^32 - 1 = 4,294,967,295 (base 10);
//          = ffffffff (base 16);
//          = 1111 1111 1111 1111 1111 1111 1111 1111 (base 2).
// The digits in hexadecimal (base 16) are 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, a, b, c, d, e, f.

double MWCUniform (unsigned int seed) {

   // Static variables retain their values between function calls.
   static unsigned int a1, a0, n1, n0, c1, c0, seeded=0;

   unsigned int p, x1, x2, x3, x4, x5, x6, sum, carry;

   // If not yet seeded, seed the MWC function.
   // Represent a, n, and c as with pairs of 16-bit numbers:
   //  a = a1 * 2^16 + a0
   //  n = n1 * 2^16 + n0
   //  c = c1 * 2^16 + c0
   if (!seeded) {
      if (!seed) {
         printf ("MWCUniform must be seeded with a postive integer.\n");
         Pause ();
      }
      seed |= 0x70007000;
      a0 = 4294967118 & 0xffff;               // first 16 bits of the multiplier
      a1 = (4294967118 & 0xffff0000) >> 16;   // second 16 bits
      n0 = seed & 0xffff;                     // first 16 bits of the seed
      n1 = (seed & 0xffff0000) >> 16;         // second 16 bits
      c0 = 1;                                 // first 16 bits of "c"
      c1 = 0;                                 // second 16 bits
      seeded = 1;
      return (0);
   }

   // Otherwise, generate the next number in the sequence.
   // Here mod 2^32 is automatic since an unsigned integer is represented by 32 bits.

   // Compute x1, x2, where a0 * n0 = x1 * 2^16 + x2.
   p = a0 * n0;
   x2 = p & 0xffff;
   x1 = (p & 0xffff0000) >> 16;

   // Compute x3, x4, where a1 * n0 + a0 * n1 = x3 * 2^16 + x4.
   p = a1 * n0 + a0 * n1;
   x4 = p & 0xffff;
   x3 = (p & 0xffff0000) >> 16;

   // Compute x5, x6, where a1 * n1 = x5 * 2^16 + x6.
   p = a1 * n1;
   x6 = p & 0xffff;
   x5 = (p & 0xffff0000) >> 16;

   // Compute next n0, n1, c0, c1, where an + c = c1 * 2^48 + c0 * 2^32 + n1 * 2^16 + n0.
   // First compute n0.
   sum = x2 + c0;
   n0 = sum & 0xffff;
   carry = (sum & 0xffff0000) >> 16;

   // Compute n1.
   sum = carry + x1 + x4 + c1;
   n1 = sum & 0xffff;
   carry = (sum & 0xffff0000) >> 16;

   // Compute c0.
   sum = carry + x3 + x6;
   c0 = sum & 0xffff;
   carry = (sum & 0xffff0000) >> 16;

   // Compute c1.
   c1 = x5 + carry;

   return ( (n0 + (n1 << 16) + 0.5) / 4294967296.0 );

}



////////////////////////////////////////////////////////////////////////////////
////// 64 bit LCG by Donald Knuth

// This LCG is designed for a 64 bit processor.  Here it is re-written to run
//    on a 32 bit processor.  Since it cannot take full advantage of 64 bit
//    processing this version is time inefficient.
// Here a = 6364136223846793005 = 22609 * 2^48 + 62509 * 2^32 + 19605 * 2^16 + 32557
//  and c = 1442695040888963407 =  5125 * 2^48 + 31614 * 2^32 + 63335 * 2^16 + 33103.

double LCG64Uniform (int unsigned seed) {

   static unsigned int a[] = {32557, 19605, 62509, 22609};
   static unsigned int x[4], y[4], *to, *from, seeded=0;
   unsigned int carry, *tmp;
   int i, j, k, done;

   if (!seeded) {
      if (!seed) {
         printf ("LCG64Uniform must be seeded with a postive integer.\n");
         Pause ();
      }
      x[1] = seed >> 16;
      x[0] = seed & 0x0000ffff;
      y[0] = y[1] = y[2] = y[3] = x[2] = x[3] = 0;
      to = x;
      from = y;
      seeded=1;
      return (0);
   }

   // Toggle the pointers.
   tmp = to;
   to = from;
   from = tmp;

   // Intialize "to" to "c".
   to[0] = 33103; to[1] = 63335; to[2] = 31614; to[3] = 5125;

   // Now multiply a*from and add it to "to".
   for (k = 0; k <= 3; k++) {
      for (i = 0; i <= k; i++) {
         // Multiply:
         to[k] += a[i] * from[k-i];
         // Carry:
         j = k;
         done = 0;
         while (!done) {
            carry = (to[j] >> 16);
            to[j] = (to[j] & 0x0000ffff);
            j++;
            if (j == 4 || carry == 0) {
               done = 1;
            } else {
               to[j] += carry;
            }
         } // while
      } // i loop
   } // k loop

   return ( (((to[3]<<16) + to[2]) + 0.5) / 4294967296.0 );

}



////////////////////////////////////////////////////////////////////////////////
// This function waits for a user-input "enter", then exits the program.
// It prevents the window from closing up before the output can be viewed.
void Pause () {

   char input[100];

   printf ("\n");
   printf ("Hit Enter to exit... ");
   fgets (input, 9, stdin);

   exit (0);

}


////////////////////////////////////////////////////////////////////////////////
// This function creates a histogram of randomly generated numbers. It is
// typically used with continuous data (not discrete), like normally distributed
// data.

double Histogram (double x,          // Item of data to be added to the histogram.
                  double a,          // Lower bound of histogram.
                  double b,          // Upper bound of histogram.
                  int n,             // Number of "buckets".
                  int done)          // 0 if not done, 1 if done.

{

   static double *freq = NULL;
   int k;
   double biggest=0, sum=0;
   FILE *fp;

   // Allocate space on the first call to this function.
   if (freq == NULL) {
      freq = (double *) calloc (n, sizeof (double));
   }

   // If not yet finished, add the current item of data to the histogram.
   if (!done) {


      // See if data is below the lower bound.
      if (x <= a) {
         freq[0] ++;
      }

      // See if data is above the upper bound.
      else if (x >= b) {
         freq[n-1] ++;
      }

      // Otherwise find the appropriate bucket.
      else {
         k = (x - a) / ((b - a) / n);
         freq[k] ++;
      }

   }


   // If finished, create the TeX files that generate the plot. This is the
   // final call to the histogram function.
    else {

      fp = fopen ("histogram.tex", "w");
      fprintf (fp, "\\input pictex\\magnification=\\magstep1\\nopagenumbers\n");
      fprintf (fp, "\\beginpicture\n");
      fprintf (fp, "\\setcoordinatesystem units <%8.3f truein, 2.5 truein>\n", 5.0 / (b-a));
      fprintf (fp, "\\setplotarea x from %8.3f to %8.3f, y from  0 to 1.0\n", a, b);
      fprintf (fp, "\\axis left\n");
      fprintf (fp, "label {}\n");
      fprintf (fp, "ticks   numbered from  0 to 1 by .2\n");
      fprintf (fp, "/\n");
      fprintf (fp, "\\axis bottom\n");
      fprintf (fp, "label {$x$}\n");
      fprintf (fp, "ticks numbered from %8.3f to %8.3f by %8.1f\n", a, b, (b-a)/n);
      fprintf (fp, "/\n");
      fprintf (fp, "\\sethistograms\n");
      fprintf (fp, "\\plot \"histogram.txt\"\n");
      fprintf (fp, "\\endpicture\\vfill\\end\n");
      fclose (fp);

      // Scale so that the biggest bucket has 1 (for graphing convenience).

      // First find the largest bucket value.
      for (k = 0; k < n; k++) {
         if (freq[k] > biggest) {
            biggest = freq[k];
            sum += freq[k];
         }
      }

      // Now re-scale all the bucket values.
      for (k = 0; k < n; k++) {
         freq[k] /= biggest;
      }

      // Compute the largest probability.
      biggest /= sum;

      // Report data to the output file.
      fp = fopen ("histogram.txt", "w");
      fprintf (fp, "%10.5f  %10.5f\n", a, 0.0);
      for (k = 0; k < n; k++) {
         fprintf (fp, "%10.5f  %10.5f\n",
                       a + (k + 1) * ((b - a) / n), freq[k]);
      }
      fclose (fp);

      // Free up the freq[] array.
      free (freq);

   }

   return (biggest);

}



////////////////////////////////////////////////////////////////////////////////
// This function creates a histogram of randomly generated numbers. It is
// typically used with discrete data, like binomially distributed data.

double DiscreteHistogram (int x,          // Data to be added to the histogram.
                          int a,          // Lower bound of histogram.
                          int b,          // Upper bound of histogram.
                          int done)       // 0 if not done, 1 if done.

{

   static double *freq = NULL;
   int k;
   double biggest=0, sum=0;
   FILE *fp;

   // Allocate space on the first call to this function.
   if (freq == NULL) {
      freq = (double *) calloc (b-a+2, sizeof (double));
   }

   // If not yet finished, add the current item of data to the histogram.
   if (!done) {

      // Truncate if data is below the lower bound.
      if (x < a) {
         x = a;
      }

      // Truncate if data is above the upper bound.
      if (x > b) {
         x = b;
      }

      // Increment the appropriate bucket.
      freq[x-a] ++;


   }


   // If finished, create the TeX files that generate the plot. This is the
   // final call to the histogram function.
   else {

      fp = fopen ("histogram.tex", "w");
      fprintf (fp, "\\input pictex\\magnification=\\magstep1\\nopagenumbers\n");
      fprintf (fp, "\\beginpicture\n");
      fprintf (fp, "\\setcoordinatesystem units <%8.3f truein, 2.5 truein>\n", 5.0 / (b-a));
      fprintf (fp, "\\setplotarea x from %d to %d, y from  0 to 1.0\n", a, b);
      fprintf (fp, "\\axis left\n");
      fprintf (fp, "shiftedto x=%f\n", a - (b-a)/10.0);
      fprintf (fp, "label {}\n");
      fprintf (fp, "ticks   numbered from  0 to 1 by .2\n");
      fprintf (fp, "/\n");
      fprintf (fp, "\\axis bottom\n");
      fprintf (fp, "label {$x$}\n");
      fprintf (fp, "ticks numbered from %d to %d by 1\n", a, b);
      fprintf (fp, "/\n");
      fprintf (fp, "\\linethickness=%ftruein\n", 1.0/(b-a));
      fprintf (fp, "\\input histogram.txt\n");
      fprintf (fp, "\\endpicture\\vfill\\end\n");
      fclose (fp);

      // Scale so that the biggest bucket has 1 (for graphing convenience).

      // First find the largest bucket.
      for (k = 0; k <= b-a; k++) {
         sum += freq[k];
         if (freq[k] > biggest) {
            biggest = freq[k];
         }
      }

      // Now scale all the buckets.
      for (k = 0; k <= b-a; k++) {
         freq[k] /= biggest;
      }

      // Compute the biggest probability.
      biggest /= sum;

      // Report data to the output file.
      fp = fopen ("histogram.txt", "w");
      for (k = 0; k <= b-a; k++) {
         fprintf (fp, "\\putrule from %d 0 to %d %f \n", a+k, a+k, freq[k]);
      }
      fclose (fp);

      // Free up the freq[] array.
      free (freq);

   }

   return (biggest);

}


////////////////////////////////////////////////////////////////////////////////
// This histogram function is specially designed for data that should be
// Normal(0,1) in distribution.  The function computes a properly scaled
// standard normal density function for comparison to the data.

void NormalHistogram (double x,          // Data to be added to the histogram.
                      int n,             // Number of "buckets".
                      int done)          // 0 if not done, 1 if done.

{

   static double *freq = NULL;
   int k;
   double a, b, z, area;
   FILE *fp;

   // Allocate space on the first call to this function.
   if (freq == NULL) {
      freq = (double *) calloc (n, sizeof (double));
   }

   // Hardwire upper and lower bounds for standard normals.
   a = -5; b = 5;

   // If adding a new piece of data...
   if (!done) {

      // See if data is below the lower bound.
      if (x <= a) {
         freq[0] ++;
      }

      // See if data is above the upper bound.
      else if (x >= b) {
         freq[n-1] ++;
      }

      // Otherwise find the appropriate bucket.
      else {
         k = (x - a) / ((b - a) / n);
         freq[k] ++;
      }

   }

   // ...otherwise, when finished, create the TeX output files for viewing.
   else {

      // Create the main TeX file.
      fp = fopen ("histogram.tex", "w");
      fprintf (fp, "\\input pictex\\magnification=\\magstep1\\nopagenumbers\n");
      fprintf (fp, "\\beginpicture\n");
      fprintf (fp, "\\setcoordinatesystem units <0.5 truein, 1.5 truein>\n", 5.0 / (b-a));
      fprintf (fp, "\\setplotarea x from -5 to 5, y from  0 to 1.0\n");
      //fprintf (fp, "\\axis left\n");
      //fprintf (fp, "label {}\n");
      //fprintf (fp, "ticks   numbered from  0 to 1 by .2\n");
      //fprintf (fp, "/\n");
      fprintf (fp, "\\axis bottom\n");
      fprintf (fp, "label {$x$}\n");
      fprintf (fp, "ticks numbered from -5 to 5 by 1\n");
      fprintf (fp, "/\n");
      fprintf (fp, "\\plot \"normal.txt\"\n");
      fprintf (fp, "\\sethistograms\n");
      fprintf (fp, "\\plot \"histogram.txt\"\n");
      fprintf (fp, "\\endpicture\\vfill\\end\n");
      fclose (fp);

      // Scale so that the area is sqrt(2pi) (for comparing to the Normal(0,1) density).
      area = 0;
      for (k = 0; k < n; k++) {
         area += freq[k] * (b-a) / n;
      }
      area /= sqrt(2*3.14159);
      for (k = 0; k < n; k++) {
         freq[k] /= area;
      }

      // Report the histogram to an output file.
      fp = fopen ("histogram.txt", "w");
      fprintf (fp, "%10.5f  %10.5f\n", a, 0.0);
      for (k = 0; k < n; k++) {
         fprintf (fp, "%10.5f  %10.5f\n",
                       a + (k + 1) * ((b - a) / n), freq[k]);
      }
      fclose (fp);

      // Report the appropriately scaled normal density function for comparison.
      fp = fopen ("normal.txt", "w");
      for (z = -5; z <= 5; z += .01) {
         fprintf (fp, "%8.3f %8.4f\n", z, exp(-z*z/2));
      }
      fclose (fp);

   }

   return;

}


// This function creates a histogram of the variable x and compares it to
// the density function for a mean 1 exponential.
void ExponentialHistogram (double x,          // data to be added to the histogram
                           int n,             // number of "buckets"
                           int done)          // 1 when done, 0 otherwise

{

   static double *freq = NULL;
   int k;
   double a, b, z, area;
   FILE *fp;

   // Allocate space on the first call to this function.
   if (freq == NULL) {
      freq = (double *) calloc (n, sizeof (double));
   }

   // Hardwire upper and lower bounds for exponential mean 1.
   a = 0; b = 5;

   // If adding a new piece of data...
   if (!done) {

      // See if data is below the lower bound.
      if (x <= a) {
         freq[0] ++;
      }

      // See if data is above the upper bound.
      else if (x >= b) {
         freq[n-1] ++;
      }

      // Otherwise find the appropriate bucket.
      else {
         k = (x - a) / ((b - a) / n);
         freq[k] ++;
      }

      // ...otherwise, when finished, create the TeX output files for viewing.
   } else { 

      // create the main TeX file
      fp = fopen ("histogram.tex", "w");
      fprintf (fp, "\\input pictex\\magnification=\\magstep1\\nopagenumbers\n");
      fprintf (fp, "\\beginpicture\n");
      fprintf (fp, "\\setcoordinatesystem units <1 truein, 1.5 truein>\n");
      fprintf (fp, "\\setplotarea x from 0 to 5, y from  0 to 1.0\n");
      fprintf (fp, "\\axis bottom\n");
      fprintf (fp, "label {$x$}\n");
      fprintf (fp, "ticks numbered from 0 to 5 by 1\n");
      fprintf (fp, "/\n");
      fprintf (fp, "\\put {$\\leftarrow$} at 5.1 %8.3f\n", exp(-b) * n / (b-a));
      fprintf (fp, "\\plot \"exponential.txt\"\n");
      fprintf (fp, "\\sethistograms\n");
      fprintf (fp, "\\plot \"histogram.txt\"\n");
      fprintf (fp, "\\endpicture\\vfill\\end\n");
      fclose (fp);

      // Scale frequencies so that the area is 1 (for comparing to the Exp(1) density).
      area = 0;
      for (k = 0; k < n; k++) {
         area += freq[k] * (b-a) / n;
      }
      for (k = 0; k < n; k++) {
         freq[k] /= area;
      }

      // Report histogram data.
      fp = fopen ("histogram.txt", "w");
      fprintf (fp, "%10.5f  %10.5f\n", a, 0.0);
      for (k = 0; k < n; k++) {
         fprintf (fp, "%10.5f  %10.5f\n",
                       a + (k + 1) * ((b - a) / n), freq[k]);
      }
      fclose (fp);

      // Report the exp(1) density function.
      fp = fopen ("exponential.txt", "w");
      for (z = 0; z <= b; z += .01) {
         fprintf (fp, "%8.3f %8.4f\n", z, exp(-z));
      }
      fclose (fp);   

   }

   return;

}


////////////////////////////////////////////////////////////////////////////////
// This program creates a histogram of the data x, and plots the Uniform[0,1]
// density function for comparison.
void UniformHistogram (double x,         // data to be added to the histogram
                      int n,             // number of "buckets"
                      int done)          // 1 when done, 0 otherwise

{

   int k;
   double a, b, area;
   FILE *fp;
   static double *freq = NULL;

   // Allocate space on the first call to this function.
   if (freq == NULL) {
      freq = (double *) calloc (n, sizeof (double));
   }

   // Hardwire upper and lower bounds for the Uniform[0,1].
   a = 0; b = 1;

   // If adding a new piece of data.
   if (!done) {

      // See if data is below the lower bound.
      if (x <= a) {
         freq[0] ++;
      }

      // See if data is above the upper bound.
      else if (x >= b) {
         freq[n-1] ++;
      }

      // Otherwise find the appropriate bucket.
      else {
         k = (x - a) / ((b - a) / n);
         freq[k] ++;
      }

   } else {  // When finished, create the TeX output files for viewing.

      // Create the main TeX file.
      fp = fopen ("histogram.tex", "w");
      fprintf (fp, "\\input pictex\\magnification=\\magstep1\\nopagenumbers\n");
      fprintf (fp, "\\beginpicture\n");
      fprintf (fp, "\\setcoordinatesystem units <5 truein, 1.5 truein>\n", 5.0 / (b-a));
      fprintf (fp, "\\setplotarea x from 0 to 1, y from  0 to 1.0\n");
      fprintf (fp, "\\axis bottom\n");
      fprintf (fp, "label {$x$}\n");
      fprintf (fp, "ticks numbered from 0 to 1 by .1\n");
      fprintf (fp, "/\n");
      fprintf (fp, "\\plot 0 1  1 1 /\n");    // the Uniform[0,1] density function
      fprintf (fp, "\\sethistograms\n");
      fprintf (fp, "\\plot \"histogram.txt\"\n");
      fprintf (fp, "\\endpicture\\vfill\\end\n");
      fclose (fp);

      // Scale so that the area is 1 (for comparing to the uniform[0,1] density).
      area = 0;
      for (k = 0; k < n; k++) {
         area += freq[k] * (b-a) / n;
      }
      for (k = 0; k < n; k++) {
         freq[k] /= area;
      }

      // Report the histogram data.
      fp = fopen ("histogram.txt", "w");
      fprintf (fp, "%10.5f  %10.5f\n", a, 0.0);
      for (k = 0; k < n; k++) {
         fprintf (fp, "%10.5f  %10.5f\n",
                       a + (k + 1) * ((b - a) / n), freq[k]);
      }
      fclose (fp);

   }

   return;

}


////////////////////////////////////////////////////////////////////////////////
// Test to see if a = b up to an allowable error of epsilon. ///////////////////
int Equal (double a, double b, double epsilon) {

   int equal;

   equal = (fabs(a-b) <= epsilon);

   return (equal);

}


////////////////////////////////////////////////////////////////////////////////
// Allocate array space for an n x m array. ////////////////////////////////////
double **Array (int n, int m) {

   int i;
   double **A;

   A = (double **) calloc (n+1, sizeof (double *));
   for (i = 0; i <= n; i++) {
      A[i] = (double *) calloc (m+1, sizeof (double));
   }

   return (A);

}


////////////////////////////////////////////////////////////////////////////////
// Normal random number generator (polar method). //////////////////////////////
double PolarNormal () {

   static int compute_a_new_pair=1;
   double static Y;

   int accepted;
   double U, V, A, B, S, W, X;

   // The flag compute_a_new_pair is 1 if both X and Y  have been used and
   //  a new pair (X,Y) must be computed.  X will then get used first and
   //  compute_a_new_pair is set to 0 until Y is also used.

   // The flag compute_a_new_pair is 0 if (X,Y) have recently been computed and
   //  only X has been used so far.  Y gets used and compute_a_new_pair
   //  is set to 1 so that a new pair will get computed with the next function call.

   if (compute_a_new_pair) {

      // Generate (A,B) uniformly distributed on the unit disk.
      accepted = 0;
      while (!accepted) {

         // Get two fresh uniforms.
         U = MTUniform (0);   V = MTUniform (0);

         // Compute A and B uniformly distributed on [-1,1] x [-1,1].
         A = -1 + 2.0 * U;
         B = -1 + 2.0 * V;

         // Accept if (A,B) lies within the unit disk.
         S = A*A + B*B;
         accepted = (S < 1);

      }

      // Compute X and Y via the polar method.
      W = sqrt ( -2*log(S) / S);
      X = A * W;
      Y = B * W;

      // Flip the flag.
      compute_a_new_pair = 0;

      // Use X.
      return (X);

   } else {

      // Flip the flag.
      compute_a_new_pair = 1;

      // Use Y.
      return (Y);

   }

}

// This function computes Psi(x) via its Taylor series expansion.  With 80
// terms the error will < 1e-10 for -6 <= x <= 6.  For values of x outside this
// range the computation is handled differently, still producing at least 9
// significant digits.
double PsiTS (double x) {

   static double scale = 1.0/sqrt(2*3.14159265358979), r[200];
   static int initialized = 0, N=80;
   double y, sum, term, w;
   int n;

   if (!initialized) {
      for (n = 1; n <= N; n++) {
         r[n] = -(2*n-1.0)/(2*n+1.0)/n/2.0;
      }
      initialized = 1;
   }

   if (x < -6) {
      y = -(scale/x) * exp(-x*x/2);
   }

   else if (x <= 6) {
      sum = term = x;
      w = x*x;
      for (n = 1; n <= N; n++) {
         term *= (w * r[n]);
         sum += term;
      }
      y = 0.5 + scale * sum;
   }

   else {
      y = 1 - (scale/x) * exp(-x*x/2);
   }

   return (y);

}

////////////////////////////////////////////////////////////////////////////////
// Normal (cumulative) distribution function evaluated at "x". /////////////////
// It runs almost 3 times as fast as the more accurate Psi() ///////////////////
// Is accurate to 7 decimal places for all values of x /////////////////////////
double Psi (double x) {

   static double    A =    0.0293868870,
                    B =    0.2161934875,
                    C =    0.6503029656,
                    D =    0.7978845608,
                    E =    0.0594864800,
                    F =    0.4160822924;

   int sign=1;
   double R;

   if (x < 0) {
      sign = -1;
      x *= -1;
   }

   x = fabs(x);
   R = (((A*x+B)*x+C)*x+D)*x / ((E*x+F)*x+1);

   // Return Psi (x)
   return (0.5 + sign * 0.5 * (1.0 - exp (-R)));

}


////////////////////////////////////////////////////////////////////////////////
// This algorithm is due to Peter John Acklam.
// It is a very good approximation of Psi^(-1).
double PsiInv (double u) {

   static double
    A1 =  -3.969683028665376e+01,
    A2 =   2.209460984245205e+02,
    A3 =  -2.759285104469687e+02,
    A4 =   1.383577518672690e+02,
    A5 =  -3.066479806614716e+01,
    A6 =   2.506628277459239e+00,
    B1 =  -5.447609879822406e+01,
    B2 =   1.615858368580409e+02,
    B3 =  -1.556989798598866e+02,
    B4 =   6.680131188771972e+01,
    B5 =  -1.328068155288572e+01,
    C1 =  -7.784894002430293e-03,
    C2 =  -3.223964580411365e-01,
    C3 =  -2.400758277161838e+00,
    C4 =  -2.549732539343734e+00,
    C5 =   4.374664141464968e+00,
    C6 =   2.938163982698783e+00,
    D1 =   7.784695709041462e-03,
    D2 =   3.224671290700398e-01,
    D3 =   2.445134137142996e+00,
    D4 =   3.754408661907416e+00,
    P0 =  0.02425,
    P1 =  0.97575;

   double N, q, r;

   // Left tail.
   if (u < P0) {
      q = sqrt(-2*log(u));
      N = (((((C1*q+C2)*q+C3)*q+C4)*q+C5)*q+C6) / ((((D1*q+D2)*q+D3)*q+D4)*q+1);
   }

   // Middle, general case.
   else if (u <= P1) {
      q = u - 0.5;
      r = q*q;
      N = (((((A1*r+A2)*r+A3)*r+A4)*r+A5)*r+A6)*q /(((((B1*r+B2)*r+B3)*r+B4)*r+B5)*r+1);
   }

   // Right tail.
   else {
      q = sqrt(-2*log(1.0-u));
      N = -(((((C1*q+C2)*q+C3)*q+C4)*q+C5)*q+C6) / ((((D1*q+D2)*q+D3)*q+D4)*q+1);
   }

   return (N);

}




////////////////////////////////////////////////////////////////////////////////
// This function generates two correlated standard normals using the
//    polar method.   They are put into X[1] and X[2].
void CorrelatedNormals (double rho, double *X) {

   // Make sure rho is in the right range.
   if (fabs(rho) > 1) {
      printf ("Rho is too big in CorrelatedNormals.\n");
      Pause ();
   }

   // Generate two independent standard normals.
   X[1] = PolarNormal ();
   X[2] = PolarNormal ();

   // Transform X[2] so that Correlation(X[1],X[2]) = rho.
   X[2] = rho * X[1] + sqrt(1.0 - rho*rho) * X[2];

   return;

}



