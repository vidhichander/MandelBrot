/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>
#include <math.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
	ComplexNumber * z = newComplexNumber(0.0, 0.0); 
	ComplexNumber * z_sq = newComplexNumber(0.0, 0.0);
	ComplexNumber * summ = newComplexNumber(0.0, 0.0);
	u_int64_t i = 1;
	while (i <= maxiters){
		freeComplexNumber(summ);
		freeComplexNumber(z_sq);
		z_sq = ComplexProduct(z, z);
		summ = ComplexSum(z_sq, point);
		if (ComplexAbs(summ) >= threshold){
			freeComplexNumber(z_sq);
			freeComplexNumber(summ);
			freeComplexNumber(z);
			return (i);
		}
		freeComplexNumber(z);
		z = newComplexNumber(Re(summ), Im(summ));
		i++;
	}
	freeComplexNumber(z);
	freeComplexNumber(summ);
	freeComplexNumber(z_sq);
	return 0;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output)
{	
	double center_real = Re(center);
	double center_im = Im(center);
	u_int64_t i = 0;
	for (double x = 0; x < (resolution*2)+1; x++){
		for (double y = 0; y < (resolution*2)+1; y ++){
			double modu = i % (u_int64_t)((resolution*2)+1);
			double real = (center_real - scale) + (scale*(modu/resolution));
			double im = (center_im + scale) - (scale*((x/resolution))); 
			ComplexNumber * array_val = newComplexNumber(real, im);
			u_int64_t iterations = MandelbrotIterations(max_iterations, array_val, threshold);
			*output = iterations;
			freeComplexNumber(array_val);
			i++;
			output++; 		
}
}
}


