#include "gemWriter.h"
#include <stdlib.h>
#include <stdio.h>
#include "colour.h"
#include <memory.h>
#define GET_INDEX(x, y) ((x) + (y)*img->width)
#define MAGIC "P6\n"
enum{
	RED = 0,
	GREEN,
	BLUE
};
struct gemImg_t{
	unsigned width;
	unsigned height;
	char (*pixels)[3];
};



Gem gemCreate(unsigned width, unsigned height){
	Gem ret = malloc(sizeof(*ret) * width * height);
	if(!ret)
		return NULL;
	ret->width = width;
	ret->height = height;
	ret->pixels = malloc(sizeof(*ret->pixels) * ret->width * ret->height);
	if(!ret->pixels){
		free(ret);
		return NULL;
	}
	return ret;
}


void gemWrite(Gem img, const char *path){
	if(!img || !path)
		return;

	FILE *file = fopen(path, "w");
	if(!file)
		return;

	fprintf(file, MAGIC"%u %u 255\n", img->width, img->height);
	fwrite(img->pixels, sizeof(*img->pixels), img->width * img->height, file);

	fclose(file);
}

void gemDestroy(Gem img){
	if(!img)
		return;
	free(img->pixels);
	free(img);
}

void gemSetBackGround(Gem img, Colour col){
	if(!img)
		return;
	for(int i = 0; i < img->width * img->height; i++){
		img->pixels[i][RED] = R(col);
		img->pixels[i][GREEN] = G(col);
		img->pixels[i][BLUE] = B(col);
	}
}
void gemSetPixel(Gem img,int x, int y, Colour col){
	if(!img)
		return;
	if(x < 0 || x>= img->width || y < 0 || y >= img->height)
		return;

	img->pixels[GET_INDEX(x, y)][RED] = R(col);
	img->pixels[GET_INDEX(x, y)][GREEN] = G(col);
	img->pixels[GET_INDEX(x, y)][BLUE] = B(col);
}
