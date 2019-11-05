/*********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"

//You don't need to call this function but it helps you understand how the arguments are passed in 
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}

//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{	
	FILE * out_file = fopen(outputfile, "w");
	if(colorfile == NULL || width < 1 || heightpercolor < 1){
		fclose(out_file);
		return 1;	
}	
	FILE * input_file = fopen(colorfile, "r");
	int * size =(int*) malloc (sizeof (int));
	uint8_t ** colors = FileToColorMap(colorfile, size);
	if (colors == NULL){
     
        fclose(out_file);
        fclose(input_file);
	return 1;

}
 	int * count =(int*) malloc (sizeof (int));
	fscanf(input_file, "%d", count);	
       fprintf(out_file, "%s %d %d %d\n", "P3", width, heightpercolor*(*count), 255);

	for (int i = 0; i < *count; i ++){
		for (int k = 0; k < heightpercolor; k ++){
			for (int l = 0; l < width; l ++){
				fprintf(out_file, "%d %d %d", colors[i][0], colors[i][1], colors[i][2]);
			 	if(l+1 != width){
					fprintf(out_file, " ");				
}
}
fprintf(out_file, "\n"); 
}

} 	
	for (int i = 0; i < *count; i ++){
		free(colors[i]);
	}
	free(count);
	free(colors);
	fclose(out_file);
	fclose(input_file);
	return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	 FILE * out_file = fopen(outputfile, "w");
        if(colorfile == NULL || width < 1 || heightpercolor < 1){
                fclose(out_file);
                return 1;
}
        FILE * input_file = fopen(colorfile, "r");
        int* size = malloc (sizeof (int));
	uint8_t ** colors = FileToColorMap(colorfile, size);
	if (colors == NULL){
        fclose(out_file);
        fclose(input_file);
        return 0;
}
	int * count = malloc(sizeof(int));
	fscanf(input_file, "%d", count);   

 fprintf(out_file, "%s %d %d %d\n", "P6", width, heightpercolor*(*count), 255);
	for (int i = 0; i < *count; i ++){
                for (int k = 0; k < heightpercolor; k ++){
                        for (int l = 0; l < width; l ++){
				char arr[3] = {colors[i][0], colors[i][1], colors[i][2]};
				fwrite(arr, 1, sizeof(arr), out_file);
}                          
		}
	}
for (int i = 0; i < *count; i ++){
                free(colors[i]);
        }
	free(count);
	free(colors);
	fclose(out_file);
	fclose(input_file);
	return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)+1];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("P3colorpalette and P6colorpalette ran without error, output should be stored in %s%s, %s%s", argv[2], P3end, argv[2], P6end);
	return 0;
}




