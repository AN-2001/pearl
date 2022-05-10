#ifndef GEM_WRITER
#define GEM_WRITER
typedef int Colour;
typedef struct gemImg_t *Gem;

Gem gemCreate(unsigned width, unsigned height);

void gemWrite(Gem img, const char *path);
void gemSetBackGround(Gem img, Colour col);
void gemSetPixel(Gem img,int x, int y, Colour col);

void gemDestroy(Gem img);
#endif // GEM_WRITER
