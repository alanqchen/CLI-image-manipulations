#include <stdlib.h>
#include "blur.h"
#include "pixel.h"
#include "imageutil.h"

/* Assumes that pixel is already defined to be an int (pixel.c0) */
/* typedef int pixel; */

bool checkNonNeg(int* mask, int maskwidth)
//@requires maskwidth % 2 == 1;
//@requires \length(mask) == maskwidth*maskwidth;
//@requires maskwidth >= 0;
{
  for(int index = 0; index < maskwidth*maskwidth; index++) 
  //@loop_invariant index >= 0 && index <= maskwidth*maskwidth;
  {
    if(mask[index] < 0) return false;
  }
  return true;
}

pixel_t checkValid(pixel_t* pixels, int index, int width, int height,int* mask, int maskWidth)
//@requires is_valid_imagesize(width, height); 
//@requires maskWidth >= 0;
//@requires maskWidth % 2 == 1;
//@requires width*height == \length(pixels);
//@requires \length(mask) == maskWidth*maskWidth;
//@requires checkNonNeg(mask, maskWidth);
{
  int hW = maskWidth/2;
  int totalRed=0;
  int totalGreen=0;
  int totalBlue=0;
  int validIndex;
  int indexRow;
  int indexCol;
  int testRow;
  int testCol;
  int total = 0;
  //pixel_t new;
  for(int indexMask = 0; indexMask < maskWidth*maskWidth; indexMask++)
  //@loop_invariant indexMask >= 0 && indexMask <= maskWidth*maskWidth;
  {
    indexRow = get_row(index, width, height);
    indexCol = get_column(index, width, height);
    testRow = get_row(indexMask, maskWidth, maskWidth)-hW+indexRow;
    testCol = get_column(indexMask,maskWidth,maskWidth)-hW+indexCol;
    if(is_valid_pixel(testRow,testCol,width,height)) {
      validIndex = get_index(testRow,testCol,width,height);
      totalRed += get_red(pixels[validIndex])*mask[indexMask];
      totalGreen += get_green(pixels[validIndex])*mask[indexMask];
      totalBlue += get_blue(pixels[validIndex])*mask[indexMask];
      total += mask[indexMask];
    }
  }
  //assert(total != 0);
  totalRed = totalRed/total;
  totalBlue = totalBlue/total;
  totalGreen = totalGreen/total;
  return make_pixel(get_alpha(pixels[index]),totalRed,totalGreen,totalBlue);
}  

pixel_t* blur(pixel_t* pixels,int width,int height,int* mask,int maskwidth)
//@requires is_valid_imagesize(width, height);
//@requires maskwidth >= 0;
//@requires maskwidth % 2 == 1;
//@requires width > 0 && height > 0 && width <= int_max() / height;
//@requires \length(pixels) == width*height;
//@requires maskwidth*maskwidth == \length(mask);
//@requires checkNonNeg(mask, maskwidth);
//@ensures \length(\result) == \length(pixels);
{
  pixel_t* result =  (pixel_t*)calloc(sizeof(pixel_t), width*height);
  for(int index = 0; index < width*height; index++) 
  //@loop_invariant index >= 0 && index <= width*height;
  {
    result[index] = checkValid(pixels, index, width, height, mask, maskwidth);
  }
  return result;
}
