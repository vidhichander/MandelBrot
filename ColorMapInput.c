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


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
	FILE * ptr = fopen(colorfile, "r");
	if (ptr == NULL){
		fclose(ptr);
		printf("No such File");
		return NULL;
	}
	fscanf(ptr, "%d", colorcount);
	uint8_t ** output = (uint8_t**)malloc (*colorcount * sizeof(uint8_t*));
	if (output == NULL){
		free(colorcount);
		free(output);
		return NULL;
}

	for (int i = 0; i < *colorcount; i ++){
		output[i] = (uint8_t*) malloc(3*sizeof(uint8_t)+1);
		if (output[i] == NULL){
			free(output);
			free(colorcount);
			return NULL;
		}

}
	uint8_t num0 = 0;
	uint8_t num1 = 0;
	uint8_t num2 = 0; 
	int val = 0;
	for (int i = 0; i < *colorcount; i++){
		val = fscanf(ptr, "%hhu %hhu %hhu\n", &num0, &num1, &num2);	
		if (val == 3){
			output[i][0]= num0;
			output[i][1] = num1;
			output[i][2] = num2;
}
	 if (val != 3) {
	fclose(ptr);
	free(colorcount);
	for (int i = 0; i < *colorcount; i++){
		free(output[i]);
}
	free(output);
free(colorcount);
	return NULL;
}
		
	}
	fclose(ptr);	
	free(colorcount);
	return output;		
}


