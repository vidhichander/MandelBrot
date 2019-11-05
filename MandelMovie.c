/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>
#include <string.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so 
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
	double f_scale, final_scale;
	for (int i = 0; i < framecount; i ++){
		final_scale = (double) exp(log(finalscale / initialscale) * (i/(framecount - 1.0)));
		f_scale = (double)(initialscale * final_scale);
		Mandelbrot(threshold, max_iterations, center, f_scale, resolution, output[i]);
}
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function: 
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order. 

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/
	if (argc != 11) {
		printf("%s: Wrong number of arguments, expecting 10\n", argv[0]);
		printUsage(argv);
		return 1;
	}
	
	double threshold, initialscale, finalscale;
	ComplexNumber* center;
	u_int64_t max_iterations, resolution;
	int framecount;
	 
	threshold = atof(argv[1]);
	max_iterations = (u_int64_t)atoi(argv[2]);
	center = newComplexNumber(atof(argv[3]), atof(argv[4]));
	initialscale = atof(argv[5]);
	finalscale = atof(argv[6]);
	framecount = atoi(argv[7]);
	resolution = (u_int64_t)atoi(argv[8]);
	char *output_folder = argv[9];
	char* colorfile = argv[10];
	int* size = (int*) calloc (1, sizeof (int));
	uint8_t ** colors = FileToColorMap(colorfile, size);
	if (colors == NULL){
		return 1;
}
	
	if (threshold <= 0 || finalscale <= 0 || initialscale <= 0 || resolution < 0 || max_iterations <= 0) {
		printf("The threshold, resolution, scale, and max_iterations must be > 0");
		printUsage(argv);
		return 1;
	}
	if (framecount > 10000 || framecount <=0){
		printf("framecount must be between 1 and 10000");
		free(colors);
		return 1;
}
	if (framecount == 1 && finalscale != initialscale){
		 printUsage(argv);
		return 1;
}	if (output_folder == NULL || colorfile == NULL){
		printUsage(argv);
		return 1;
}

	


	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space. 
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/
	 u_int64_t ** output = (u_int64_t**) calloc (framecount, sizeof (u_int64_t*));
	if (output!= NULL){
		for (int x = 0; x < framecount; x++){
			output[x] = (u_int64_t*) malloc (pow(((resolution*2)+1),2) * sizeof(u_int64_t));
			if (output[x] == NULL){
				return 1;
			}
		}

	}
	MandelMovie(threshold, max_iterations, center, initialscale, finalscale, framecount, resolution, output);


	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/
	int width_height = (resolution*2)+1;	
	FILE * input_file = fopen(colorfile, "r");
	int* len = malloc (sizeof (int));
         fscanf(input_file, "%d", len);
	fclose(input_file);
	for (int i = 0; i < framecount; i++){
		char buffer[40] = {0};
		sprintf(buffer, "%s/frame%05d.ppm", output_folder, i);
		FILE * out = fopen(buffer, "w+");
		fprintf(out, "%s %d %d %d\n", "P6", width_height, width_height, 255);
		for (int j = 0; j < pow(((resolution*2)+1),2); j ++){
			int val = output[i][j];
			if (output[i][j] == 0){
				   char arr[3] = {0};
				fwrite(arr, 1, sizeof(arr), out);
			}else{
			int position = (val%*len) - 1;
			if (position == -1){
				position = *len - 1;
			}
			char arr[3] = {colors[position][0], colors[position][1], colors [position][2]};
			fwrite(arr, 1, sizeof(arr), out);
	}}
	fclose(out);
	}	

	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/	
	for (int i = 0; i < *len; i++){
		free(colors[i]);

}	
	free(colors);
	
	for (int i = 0; i < framecount; i ++){
		free(output[i]);
}
	
	freeComplexNumber(center);
	free(output);
	free(len);

	return 0;
}

