#include <stdio.h>
#include "colour.h"
#include "boilerplate.h"
#include "config.h"
#include <math.h>


int main(int argc, const char *argv[]){
	parseArgs(argc, argv);
	const char *path = (pathi != -1 ? argv[pathi] : "out");	
	const char *palettePath = (palettei != -1 ? argv[palettei] : "default.csv");	

	Palette p = paletteLoad(palettePath);
	if(!p){
		fprintf(stderr, "couldn't load palette\n");
		return 1;
	}
	paletteSet(p);

	Gem img = gemCreate(WIDTH, HEIGHT);
	if(!img){
		fprintf(stderr, "couldn't open image\n");
		return 1;
	}
	for(int x = 0; x < WIDTH; x++){
		for(int y = 0; y < HEIGHT; y++){
			float d = sqrt((x - (WIDTH/2)) * (x - (WIDTH/2)) + (y - (HEIGHT/2))*(y - (HEIGHT/2)));
			setCurrentColour(paletteInterp((d / (sqrt(WIDTH*WIDTH + HEIGHT*HEIGHT)/10)) * PALETTE_MAX_INDEX));
			fillPixel(img, x , y, USE_CUSTOM_COLOUR);
		}
	}
	outputImage(img, path, ONE_IMG);
	paletteFree(p);
	gemDestroy(img);
	return 0;
}
