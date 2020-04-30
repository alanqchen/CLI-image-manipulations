/* edge, implementation file
 * C++, 2018
 * Alan Chen
 *
 * Performs the edge transformation
 * using a mask
 */

#include <math.h>
#include "edge.h"
#include "pixel.h"
#include "imageutil.h"
#include "emboss.h"
/* Assumes that pixel is already defined to be an int (pixel.c0) */
/* typedef int pixel; */

bool checkNonNegEdge(int* mask, int maskwidth)
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

int checkValidEdge(int* pixels, int index, int width, int height,
int* mask, int maskWidth)
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
      total += pixels[validIndex]*mask[indexMask];
    }
  }
  return total;
}  

//Takes in grayscale pixels ONLY
int* emboss(int* pixels,int width,int height,int* mask,int maskwidth)
//@requires is_valid_imagesize(width, height);
//@requires maskwidth >= 0;
//@requires maskwidth % 2 == 1;
//@requires width > 0 && height > 0 && width <= int_max() / height;
//@requires \length(pixels) == width*height;
//@requires maskwidth*maskwidth == \length(mask);
//@requires checkNonNeg(mask, maskwidth);
//@ensures \length(\result) == \length(pixels);
{
  int* result =  (int*)calloc(sizeof(int), width*height);
  for(int index = 0; index < width*height; index++) {
    result[index] = checkValidEdge(pixels, index, width, height, maskX, maskwidth);
  }
  return result;
}
