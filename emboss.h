#include <math.h>
#include "pixel.h"
#include "imageutil.h"

bool checkNonNegEdge(int* mask, int maskwidth);
int checkValidEdge(int* pixels, int index, int width, int height, int* mask, int maskWidth);
int* emboss(int* pixels,int width,int height,int* maskX,int* maskY,int maskwidth);
