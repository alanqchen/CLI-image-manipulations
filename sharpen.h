#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include "pixel.h"
#include "imageutil.h"

pixel_t* sharpen(pixel_t* pixels, int width, int height, int strength, int threshold, int* mask);
