#include <stdlib.h>
#include "pixel.h"
#include "imageutil.h"
#include "subtraction.h"

int checkBounds(int value) {
    if(value > 255) {
        return 255;
    } else if(value < 0) {
        return 0;
    }
    return value;
 }

pixel_t* subtraction(pixel_t* image1, pixel_t* image2, int weight, int width, int height) {
    pixel_t* result = (pixel_t*)calloc(sizeof(pixel_t), width*height);
    for(int i=0; i < width*height; i++) {
        int red = checkBounds(get_red(image1[i]) - get_red(image2[i]) * weight);
        int green = checkBounds(get_green(image1[i]) - get_green(image2[i]) * weight);
        int blue = checkBounds(get_blue(image1[i]) - get_blue(image2[i]) * weight);
        result[i] = make_pixel(255, red, green ,blue);
    }
    return result;
}
