/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	int sR = 0, sG = 0, sB = 0;
	for(int i=row-1; i<=row+1; i++){
		for(int j=col-1; j<=col+1; j++){
			int r = (i + image->rows) % image->rows; 
			int c = (j + image->cols) % image->cols; 
			if(image->image[r][c].R > 0) {
				if(i==row && j==col) sR+=9;
				else sR++;
			}
			if(image->image[r][c].G > 0) {
				if(i==row && j==col) sG+=9;
				else sG++;
			}
			if(image->image[r][c].B > 0) {
				if(i==row && j==col) sB+=9;
				else sB++;
			}
		}
	}
	Color *color = (Color *)malloc(sizeof(Color *));
	color->R = ((rule >> sR) & 1)*255;
	color->G = ((rule >> sG) & 1)*255;
	color->B = ((rule >> sB) & 1)*255;
	return color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Color **next_image = (Color **) malloc(image->rows * sizeof(Color *));
	if(next_image == NULL) {
		printf("Error allocating memory for encrypted image. \n");
		exit(-1);
	}
	for(uint32_t i = 0; i < image->rows; i++) {
		next_image[i] = (Color *) malloc(image->cols * sizeof(Color));
		if(next_image[i] == NULL) {
			printf("Error allocating memory for encrypted image row %u. \n", i);
			for(uint32_t j = 0; j < i; j++) {
				free(next_image[j]);
			}
			free(next_image);
			exit(-1);
		}
		for(uint32_t j = 0; j < image->cols; j++) {
			Color *color = evaluateOneCell(image, i, j, rule);
			next_image[i][j] = *color;
			free(color);
		}
	}
	Image *retImage = (Image *) malloc(sizeof(Image));
	if(retImage == NULL) {
		printf("Error allocating memory for return image. \n");
		for(uint32_t i = 0; i < image->rows; i++) {
			free(next_image[i]);
		}
		free(next_image);
		exit(-1);
	}
	retImage->image = next_image;
	retImage->rows = image->rows;
	retImage->cols = image->cols;	
	return retImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	char *filename = argv[1];
	char *rule_str = argv[2];
	uint32_t rule = (uint32_t)strtoul(rule_str, NULL, 0);
	Image *image = readData(filename);
	Image *next_image = life(image, rule);
	writeData(next_image);
	freeImage(image);
	freeImage(next_image);
	return 0;
}
