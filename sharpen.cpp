#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include "pixel.h"
#include "imageutil.h"
#include "blur.h"
#include "contrast.h"
#include "subtraction.h"
#include "sharpen.h"

pixel_t* sharpen(pixel_t* pixels, int width, int height, int strength, int threshold, int* mask) {
    pixel_t* result = (pixel_t*)calloc(sizeof(pixel_t), width*height);

    pixel_t* blurPixels = blur(pixels, width, height, mask, 3); //store result in new pixel array
    pixel_t* unsharpMaskPixels = subtraction(pixels, blurPixels, 1, width, height);
    pixel_t* contrastPixels = contrast(pixels, width, height, strength);
    for(int i=0; i < width*height; i++) {
        get_row(i, width, height);
        get_column(i, width, height);
        int redUM  = get_red(unsharpMaskPixels[i]);
        int greenUM  = get_green(unsharpMaskPixels[i]);
        int blueUM  = get_blue(unsharpMaskPixels[i]);
        double brightness = (redUM / 255.0) * 0.3 + (greenUM / 255.0) * 0.59 + (blueUM / 255.0) * 0.11;
        //double thresholdPercent = (double)strength/255;
        int redC  = get_red(contrastPixels[i]);
        int greenC  = get_green(contrastPixels[i]);
        int blueC  = get_blue(contrastPixels[i]);
        int red  = get_red(pixels[i]);
        int green  = get_green(pixels[i]);
        int blue  = get_blue(pixels[i]);
        double difference = sqrt((redC-red)*(redC-red)+(greenC-green)*(greenC-green)+(blueC-blue)*(blueC-blue));
        double differencePercent = (double)difference/sqrt(255*255+255*255+255*255);
        double diffenceScaled = (double)differencePercent*255;
        double delta = diffenceScaled * brightness;
        if(delta >= threshold-.1) {
            result[i] = contrastPixels[i];
        } else {
            result[i] = pixels[i];
        }
        //double brightness = (red / 255.0) * 0.3 + (green / 255.0) * 0.59 + (blue / 255.0) * 0.11;
    }
    return result;
}