#ifndef COLOUR_H
#define COLOUR_H
#include "gemWriter.h"
#define USE_CUSTOM_COLOUR -1
#define PALETTE_COUNT (paletteGetCount())
#define PALETTE_MAX_INDEX (paletteGetCount() - 1)
#define R(c) (((c >> 16) & 0xff))
#define G(c) (((c >> 8) & 0xff))
#define B(c) (((c) & 0xff))
#define PACK(r, g, b) (((b) & 0xff) | ((g & 0xff) << 8) | ((r & 0xff) << 16)) 
typedef int Colour;
typedef int Cindex;
typedef struct palette_t *Palette;

void setBackground(Gem img, Cindex index);
void fillPixel(Gem img, int x, int y, Cindex index);
//void fillRect(Gem *img, int x, int y, int w, int h, Cindex index);
//void fillCircle(Gem *img, int x, int y, int r, Cindex index);
void setCurrentColour(Colour c);
Colour colourCreate(int r, int g, int b);

// loads a palette from a CSV file
Palette paletteLoad(const char *csvPath);

// interpolate between c1 and c2
Colour linearInterp(double t, Cindex c1, Cindex c2);
Colour bezierInterp(double t, Cindex c1, Cindex c2, double y0, double y1);
// interpolate accross the whole palette
Colour paletteInterp(float v);

// frees a palette
void paletteFree(Palette p);

// sets the current palette
void paletteSet(Palette p);

// gets the number of colours in the current palette
int paletteGetCount();

#endif // COLOUR_H
