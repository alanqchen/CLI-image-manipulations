#include <stdlib.h>
#include "contrast.h"
#include "pixel.h"

// https://www.dfstudios.co.uk/articles/programming/image-programming-algorithms/image-processing-algorithms-part-5-contrast-adjustment/
int truncateContrast(int value)
{
    if(value < 0) return 0;
    if(value > 255) return 255;

    return value;
}

pixel_t* contrast(pixel_t* pixels, int width, int height, int contrastLevel) {
    double factor = (double)(259*(contrastLevel+255))/(double)(255*(259-contrastLevel));
    pixel_t* result = (pixel_t*)calloc(sizeof(pixel_t), width*height);
    for(int i=0; i<width*height; i++) {
        int red = truncateContrast(factor*(get_red(pixels[i])-128)+128);
        int green = truncateContrast(factor*(get_green(pixels[i])-128)+128);
        int blue = truncateContrast(factor*(get_blue(pixels[i])-128)+128);
        result[i] = make_pixel(get_alpha(pixels[i]), red, green, blue);
    }
    return result;
}
