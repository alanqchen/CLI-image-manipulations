
/* brightness, implementation file
 * C++, 2018
 * Alan Chen
 *
 * 
 * Provides brightness functionality
 */

#include <stdlib.h>
#include "pixel.h"

int truncate(int value)
{
    if(value < 0) return 0;
    if(value > 255) return 255;

    return value;
}

pixel_t* brightness(pixel_t* pixels, int width, int height, int brightnessLevel) {
    pixel_t* result = (pixel_t*)calloc(sizeof(pixel_t), width*height);
    for(int i=0; i<width*height; i++) {
        int red = truncate(get_red(pixels[i])+brightnessLevel);
        int green = truncate(get_green(pixels[i])+brightnessLevel);
        int blue = truncate(get_blue(pixels[i])+brightnessLevel);
        result[i] = make_pixel(255, red, green, blue);
    }
    return result;
}
