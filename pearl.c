#include <stdio.h>
#include "colour.h"
#include "boilerplate.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define GET_INDEX(x, y) ((x) + (y) * WIDTH)
#define A_ 0
#define B_ 1
#define DA 1.0
#define DB 0.5
#define k 0.06
#define f 0.03
//#define k 0.0649
//#define f 0.0367
#define DT 1
typedef struct gridCell_t{
	float chems[2];
}Cell;
Cell *cells;


void performIteration();
void colourGrid(Gem img);
int main(int argc, const char *argv[]){
	srand(time(NULL));
	parseArgs(argc, argv);
	const char *path = (pathi != -1 ? argv[pathi] : "out");	
	const char *palettePath = (palettei != -1 ? argv[palettei] : "default.csv");	
	cells = malloc(WIDTH * HEIGHT * sizeof(*cells));
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
	for(int i = 0; i < WIDTH * HEIGHT; i++){
		cells[i].chems[A_] = 1;
		cells[i].chems[B_] = 0;
	}

//	for(int i = 0; i < 1000; i++){
//		int x = rand() % WIDTH;
//		int y = rand() % WIDTH;
//
//		cells[GET_INDEX(x, y)].chems[B] = 1;
//	}
	for(int x = WIDTH/2 - 15; x < WIDTH/2 + 15; x++){
		for(int y = HEIGHT/2 - 15; y < HEIGHT/2 + 15; y++){
			cells[GET_INDEX(x, y)].chems[B_] = 1;
		}
	}
	for(int i = 0; i < 10000; i++){
		performIteration();
		printProgress(i, 10000);
		if( i % 10 == 0){
			colourGrid(img);	
			outputImage(img, path, i / 10, 10000 / 10);
		}
	}

	paletteFree(p);
	gemDestroy(img);
	free(cells);
	return 0;
}

float getVal(int x, int y, int indx){
//	if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
//		return 0;
	x = (x + WIDTH) % WIDTH;
	y = (y + HEIGHT) % HEIGHT;
	int index = GET_INDEX(x, y);
	return cells[index].chems[indx];
}
float getLap(int x, int y, int indx){
	float n[4];
	float d[4];
	n[0] = getVal(x - 1, y, indx);
	n[1] = getVal(x + 1, y, indx);
	n[2] = getVal(x , y + 1, indx);
	n[3] = getVal(x , y - 1, indx);

	d[0] = getVal(x - 1, y - 1, indx);
	d[1] = getVal(x + 1, y + 1, indx);
	d[2] = getVal(x - 1 , y + 1, indx);
	d[3] = getVal(x + 1 , y - 1, indx);
	float c = getVal(x, y, indx);
	
	float ns = 0, ds = 0;
	for(int i = 0; i < 4; i++)
		ns += n[i] * 0.2;

	for(int i = 0; i < 4; i++)
		ds += d[i] * 0.05;

	return ns + ds + c * -1;
}
void performIteration(){
	Cell *deltas = malloc(sizeof(*deltas) * WIDTH * HEIGHT);


	for(int y = 0; y < HEIGHT; y++){
		for(int x = 0; x < WIDTH; x++){
			int index = GET_INDEX(x, y);
			float currA = getVal(x, y, A_);
			float currB = getVal(x, y, B_);
			float LA = getLap(x, y, A_);
			float LB = getLap(x, y, B_);
			float deltaA = DA * LA - currA * (currB * currB) + f * (1 - currA);
			float deltaB = DB * LB + currA * (currB * currB) - (k + f) * currB;
			deltas[index].chems[A_] = deltaA;
			deltas[index].chems[B_] = deltaB;
		}
	}

	for(int i = 0; i < WIDTH * HEIGHT; i++){
		cells[i].chems[A_] += deltas[i].chems[A_] * DT;
		cells[i].chems[B_] += deltas[i].chems[B_] * DT;

		cells[i].chems[A_] = fmin(fmax(0, cells[i].chems[A_]), 1);
		cells[i].chems[B_] = fmin(fmax(0, cells[i].chems[B_]), 1);

	}
	free(deltas);
}

void colourGrid(Gem img){

	float max = 0;

	for(int x = 0; x < WIDTH; x++){
		for(int y = 0; y < HEIGHT; y++){
			if(getVal(x, y, B_) > max)
				max = getVal(x, y, B_);
		}
	}
	for(int x = 0; x < WIDTH; x++){
		for(int y = 0; y < HEIGHT; y++){
			float p;
			if(max < 1e-4)
				p = 0;
			else
				p = getVal(x, y, B_) / max;
			setCurrentColour(paletteInterp(p * PALETTE_MAX_INDEX));
			fillPixel(img, x, y, USE_CUSTOM_COLOUR);
		}
	}
}



