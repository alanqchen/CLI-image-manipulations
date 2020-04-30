/* oil, header file
 * C++, 2018
 * Alan Chen
 *
 * Performs the oil transformation
 * using a mask
 */

#include <stdlib.h>
#include <math.h>
#include "pixel.h"
#include "imageutil.h"

pixel_t checkValidOil(pixel_t* pixels, int index, int width, int height, int maskWidth, int intensityLevel);
pixel_t* oil(pixel_t* pixels, int width, int height, int maskWidth, int intensityLevel);
