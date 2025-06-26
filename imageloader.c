/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Error reading file. \n");
		return NULL;
	}
	char buf[5];
	uint32_t n,m,g;
	fscanf(fp, "%s %" SCNu32 " %" SCNu32 " %" SCNu32, buf, &n, &m, &g);
	if(strcmp(buf, "P3") != 0){
		printf("Error: Not a P3 file. \n");
		fclose(fp);
		return NULL;
	}
	Color **image = (Color **) malloc(n * sizeof(Color *));
	if(image == NULL) {
		printf("Error allocating memory for image. \n");
		fclose(fp);
		return NULL;
	}
	for(uint32_t i = 0; i < n; i++) {
		image[i] = (Color *) malloc(m * sizeof(Color));
		if(image[i] == NULL) {
			printf("Error allocating memory for image row %u. \n", i);
			for(uint32_t j = 0; j < i; j++) {
				free(image[j]);
			}
			free(image);
			fclose(fp);
			return NULL;
		}
		for(uint32_t j = 0; j < m; j++) {
			fscanf(fp, "%" SCNu8 " %" SCNu8 " %" SCNu8, &image[i][j].R, &image[i][j].G, &image[i][j].B);
		}
	}
	fclose(fp);
	Image *img = (Image *) malloc(sizeof(Image));
	if(img == NULL) {
		printf("Error allocating memory for Image struct. \n");
	}
	else{
		img->image = image;
		img->rows = n;
		img->cols = m;		
	}
	return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	if(image == NULL) {
		printf("No image to write. \n");
		return;
	}
	//printf("...where %u is the number of cols, and %u is the number of rows.\n", image->cols, image->rows);
	printf("P3\n");
	printf("%u %u\n", image->rows, image->cols);
	printf("255\n");
	for(uint32_t i = 0; i < image->rows; i++) {
		for(uint32_t j = 0; j < image->cols; j++) {
			printf("%3" PRIu8 " %3" PRIu8 " %3" PRIu8, image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
			if(j < image->cols - 1) {
				printf("   ");
			}
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	if(image == NULL) return;
	for(uint32_t i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
	image = NULL;
}