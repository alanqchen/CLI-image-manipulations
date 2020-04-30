/* oil, implementation file
 * C++, 2018
 * Alan Chen
 *
 * Performs the oil transformation
 * using a mask
 */

//Reference: https://www.codeproject.com/Articles/471994/OilPaintEffect

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
#include "pixel.h"
#include "imageutil.h"
#include "oil.h"

using namespace std;

//largely uses blur modified logic

//struct for storing intensity data(total red, green, blue, and number of occurences)
typedef struct intensityData* data_t;
typedef struct intensityData idata;
struct intensityData {
    int totalR;
    int totalG;
    int totalB;
    int occurences;
};

pixel_t checkValidOil(pixel_t* pixels, int index, int width, int height, int maskWidth, int intensityLevel)
//@requires is_valid_imagesize(width, height); 
//@requires maskWidth >= 0;
//@requires maskWidth % 2 == 1;
//@requires width*height == \length(pixels);
//@requires \length(mask) == maskWidth*maskWidth;
//@requires checkNonNeg(mask, maskWidth);
{
  int hW = maskWidth/2;
  int validIndex;
  int indexRow;
  int indexCol;
  int testRow;
  int testCol;
  
  //Possible intensity values: 0 to intensity level(inclusive)
  data_t data = (data_t)calloc(sizeof(idata), intensityLevel+1);
  for(int i=0; i < intensityLevel+1; i++) {
    data[i].totalR = 0;
    data[i].totalG = 0;
    data[i].totalB = 0;
    data[i].occurences = 0;
  }

  for(int indexMask = 0; indexMask < maskWidth*maskWidth; indexMask++) //loop through mask
  //@loop_invariant indexMask >= 0 && indexMask <= maskWidth*maskWidth;
  {
    indexRow = get_row(index, width, height); //get test positiion row and column
    indexCol = get_column(index, width, height);
    testRow = get_row(indexMask, maskWidth, maskWidth)-hW+indexRow;
    testCol = get_column(indexMask,maskWidth,maskWidth)-hW+indexCol;
    if(is_valid_pixel(testRow,testCol,width,height)) { //check if valid pixel(in bounds)
      validIndex = get_index(testRow,testCol,width,height);
      int red = get_red(pixels[validIndex]);
      int blue = get_blue(pixels[validIndex]);
      int green = get_green(pixels[validIndex]);
      int intensity = (((red+green+blue)/3.0) * intensityLevel) / 255;
      data[intensity].totalR += red; //Add values to total in the pixel's intensity
      data[intensity].totalG += green;
      data[intensity].totalB += blue;
      data[intensity].occurences += 1;
    }
  }
  /* Analyze which intensity happened the most */
  int mostCommonIntensity = 0;
  int mostCommonIntensityOccurences = 0;
  for(int i=0; i < intensityLevel+1; i++) {
      if(data[i].occurences > mostCommonIntensityOccurences) {
          mostCommonIntensity = i;
          mostCommonIntensityOccurences = data[i].occurences;
      }
  }
  int averageRed = data[mostCommonIntensity].totalR/mostCommonIntensityOccurences;
  int averageGreen = data[mostCommonIntensity].totalG/mostCommonIntensityOccurences;
  int averageBlue = data[mostCommonIntensity].totalB/mostCommonIntensityOccurences;
  free(data);
  return make_pixel(255,averageRed,averageGreen,averageBlue);
}  

pixel_t* oil(pixel_t* pixels, int width, int height, int maskWidth, int intensityLevel) {
    pixel_t* result =  (pixel_t*)calloc(sizeof(pixel_t), width*height);
    for(int index = 0; index < width*height; index++) 
    //@loop_invariant index >= 0 && index <= width*height;
    {
        result[index] = checkValidOil(pixels, index, width, height, maskWidth, intensityLevel);
    }
    return result;
}
