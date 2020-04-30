#include <stdlib.h>
#include "art.h"
#include "pixel.h"
#include "imageutil.h"

pixel_t* art(int* pixels, int width, int height, int cutoffLevel) {
    pixel_t* result = (pixel_t*)calloc(sizeof(pixel_t), width*height*4);
    for(int index=0; index < width*height; index++) {
        int row = get_row(index, width, height);
        int col = get_column(index, width, height);
        if(pixels[index] > cutoffLevel) {
            result[get_index(row, col, width*2, height*2)] = make_pixel(255,220,20,60); //Top-left Red
            result[get_index(row, col+width, width*2, height*2)] = make_pixel(255,173,216,230); //Top-right blue
            result[get_index(row+height, col, width*2, height*2)] = make_pixel(255,255,127,80); //Bottom-left orange
            result[get_index(row+height, col+width, width*2, height*2)] = make_pixel(255,255,20,147); //Bottom-right pink
        } else {
            result[get_index(row, col, width*2, height*2)] = make_pixel(255,255,255,255); //Top-left
            result[get_index(row, col+width, width*2, height*2)] = make_pixel(255,255,255,255); //Top-right
            result[get_index(row+height, col, width*2, height*2)] = make_pixel(255,255,255,255); //Bottom-left
            result[get_index(row+height, col+width, width*2, height*2)] = make_pixel(255,255,255,255); //Bottom-right
        }
    }
    return result;
}
