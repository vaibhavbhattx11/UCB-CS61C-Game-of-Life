/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color *color = (Color *) malloc(sizeof(Color));
	if(color == NULL) {
		printf("Error allocating memory for color. \n");
		exit(-1);
	}
	uint8_t lsb = image->image[row][col].B & 1; 
	color->R = color->G = color->B = lsb * 255;
	return color; 
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Color **encryptedImage = (Color **) malloc(image->rows * sizeof(Color *));
	if(encryptedImage == NULL) {
		printf("Error allocating memory for encrypted image. \n");
		exit(-1);
	}
	for(uint32_t i = 0; i < image->rows; i++) {
		encryptedImage[i] = (Color *) malloc(image->cols * sizeof(Color));
		if(encryptedImage[i] == NULL) {
			printf("Error allocating memory for encrypted image row %u. \n", i);
			for(uint32_t j = 0; j < i; j++) {
				free(encryptedImage[j]);
			}
			free(encryptedImage);
			exit(-1);
		}
		for(uint32_t j = 0; j < image->cols; j++) {
			Color *color = evaluateOnePixel(image, i, j);
			encryptedImage[i][j] = *color;
			free(color);
		}
	}
	Image *retImage = (Image *) malloc(sizeof(Image));
	if(retImage == NULL) {
		printf("Error allocating memory for return image. \n");
		for(uint32_t i = 0; i < image->rows; i++) {
			free(encryptedImage[i]);
		}
		free(encryptedImage);
		exit(-1);
	}
	retImage->image = encryptedImage;
	retImage->rows = image->rows;
	retImage->cols = image->cols;	
	return retImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	if(argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return -1;
	}
	char *filename = argv[1];
	Image *image = readData(filename);
	Image *encryptedImage = steganography(image);
	writeData(encryptedImage);
	freeImage(image);
	freeImage(encryptedImage);
	return 0;
}
