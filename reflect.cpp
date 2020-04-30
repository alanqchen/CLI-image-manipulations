#include <stdlib.h>
#include "reflect.h"
#include "pixel.h"
#include "imageutil.h"

/* Assumes that pixel is already defined to be an int (pixel.c0) */
/* typedef int pixel; */

pixel_t* reflect(pixel_t* pixels, int width, int height)
//@requires width > 0 && height > 0 && width <= int_max() / height;
//@requires \length(pixels) == width*height;
//@ensures \length(\result) == (width*2)*(height*2);
//@ensures \length(\result) != \length(pixels);
{
  int len = (width*2)*(height*2);
  int row;
  int col;
  pixel_t* result = (pixel_t*)calloc(sizeof(pixel_t), (width*2)*(height*2));
  for(int index = 0; index < width*height; index++) 
  //@loop_invariant index >= 0 && index <= width*height;
  {
    row = index / width; col = (index % width) + width;
    result[get_index(row, col, width*2, height*2)] = pixels[index];
    col = (width*height - 1 - index) % width;
    result[get_index(row, col, width*2, height*2)] = pixels[index];
    row = (height*2 - 1) - index / width; col = (width*height-1-index) % width;
    result[get_index(row, col, width*2, height*2)] = pixels[index];
    col = (index % width) + width;
    result[get_index(row, col, width*2, height*2)] = pixels[index];
  }
  return result;
}
