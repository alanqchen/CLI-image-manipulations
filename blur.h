#include "pixel.h"
#include "imageutil.h"

bool checkNonNeg(int* mask, int maskwidth);
pixel_t checkValid(pixel_t* pixels, int index, int width, int height,
int* mask, int maskWidth);
pixel_t* blur(pixel_t* pixels,int width,int height,int* mask,int maskwidth);
