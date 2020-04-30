
/* Image manipulations, main file
 * C++, 2018
 * Alan Chen
 *
 * Performs image manipulations
 * based on the selection chosen
 */

//Compile: g++ `Magick++-config --cxxflags --cppflags` -o imagemanipulate pixel.h pixel.cpp imageutil.h imageutil.cpp reflect.h reflect.cpp blur.h blur.cpp contrast.h contrast.cpp brightness.h brightness.cpp edge.h edge.cpp art.h art.cpp subtraction.h subtraction.cpp sharpen.h sharpen.cpp oil.h oil.cpp main.cpp `Magick++-config --ldflags --libs`

//References: https://stackoverflow.com/questions/47781396/imagemagick-c-version-7-modify-pixel-value-in-blank-image
// https://stackoverflow.com/questions/7678511/getting-pixel-color-with-magick
// http://old.cescg.org/CESCG97/boros/#_Toc385678836
// https://softwarebydefault.com/2013/06/29/oil-painting-cartoon-filter/

#include <Magick++.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include "pixel.h"
#include "imageutil.h"
#include "reflect.h"
#include "blur.h"
#include "contrast.h"
#include "brightness.h"
#include "edge.h"
#include "art.h"
#include "subtraction.h"
#include "sharpen.h"
#include "oil.h"

using namespace Magick;

using namespace std;

int main(int argc, char **argv) {
  try {
    /* INITIALIZATION AND GET IMAGE */
    InitializeMagick(*argv);
    string path;
    cout << "Enter image name/path: ";
    cin >> path;
    cout << "Parsing image..." << flush;
    Image sourceImage(path); //create Magick image
    int width = sourceImage.columns(); //get width + height
    int height = sourceImage.rows();
    Quantum *sourcePixels = sourceImage.getPixels(0, 0, width, height); //get pixel data
    pixel_t* pixels = (pixel_t*)calloc(sizeof(pixel_t), width*height); //create our pixel type
    //convert from image magick data to imageutil data(easier to work with)
    for(int i = 0; i < width*height; i++) {
      int row = get_row(i, width, height);
      int col = get_column(i, width, height);
      size_t offset = (width * row + col);
      size_t mainOffset = sourceImage.channels() * offset;
      int alpha;
      if(sourceImage.channels() != 4) { //check if there is an alpha channel
        alpha = 255; //if not, set to 255
      } else {
        alpha = sourcePixels[mainOffset + 3]; //if there is, set actual value
      }
      int red = sourcePixels[mainOffset + 0]; //get red channel
      int green = sourcePixels[mainOffset + 1]; //get green channel
      int blue = sourcePixels[mainOffset + 2]; //get blue channel
      pixels[i] = make_pixel(alpha, red, green, blue); //create our pixel and store it
    }
    cout << "Done.\n" << flush;
    /* DISPLAY OPTIONS/MENU */
    int choice;
    do {
      string line1 = "Image: " + path + " Channels: " + to_string(sourceImage.channels()) + "\n"; //data line 1
      string line2 = "Width: " + to_string(width) + " Height: " + to_string(height) + " Total Pixels: " + to_string(width*height) + "\n"; //data line 2
      string border; //string to store border
      int borderLen; //store border length
      if(line1.length() > line2.length()) { //find border length based on the longest data line
        borderLen = line1.length()-1;
      } else {
        borderLen = line2.length()-1;
      }
      for(int i = 0; i < borderLen; i++) { //create border
        border += "=";
      }
      border += "\n";
      cout << border << line1 << line2 << border; //start of displaying menu
      cout << "1. Get a pixel's channel values\n";
      cout << "2. Reflection across all axis\n";
      cout << "3. Blur\n";
      cout << "4. Adjust Contrast\n";
      cout << "5. Adjust Brightness\n";
      cout << "6. Edge Detection\n";
      cout << "7. Stamp Art\n";
      cout << "8. Sharpen\n";
      cout << "9. Oil Painting\n";
      cout << "10. Emboss Image(Not Finished)\n";
      cout << "11. Segmentation(Not Finished)\n";
      cout << "12. Quit\n";
      cout << "Selection: ";
      cin >> choice;
      switch(choice) {
        case 1: { //Get a pixel's channel values
          int r,c; //store row and column choice
          cout << "Note: indexes start at [0,0] in the top-left\n";
          cout << "Enter column: "; //inputs
          cin >> c;
          cout << "Enter row: ";
          cin >> r;
          int index = get_index(r, c, width, height); //find index in pixel array
          pixel_t get = pixels[index]; //get pixel data and display
          cout << "======== Pixel at [" + to_string(c) + "," + to_string(r) + "] ========\n";
          cout << "Alpha*: " << get_alpha(get) << endl;
          cout << "Red: " << get_red(get) << endl;
          cout << "Green: " << get_green(get) << endl;
          cout << "Blue: " << get_blue(get) << endl;
          cout << "*-May not have an alpha channel\n";
          cout << "================================\n";
          break;
        }
        case 2: { //Reflection across all axis
          string destination; //store destination path
          cout << "Enter destination file/path, nothing for default(imgs/reflect.png): ";
          cin.ignore();
          getline(cin, destination);
          if(destination == "") {
            destination = "imgs/reflect.png"; //default path
          }
          string dimensions = to_string(width*2) + "x" + to_string(height*2); //create dimension string
          cout << "New Dimensions: " + dimensions << endl;
          cout << "Writing image to " + destination + "..." << flush; //display progress
          pixel_t* newPixels = reflect(pixels, width, height); //store result in new pixel array
          Image newImage((Geometry)dimensions, "transparent"); //create new Image Magick Image to write to
          Quantum *newImagePixels = newImage.getPixels(0, 0, width*2, height*2); //Get pixel data in new Image
          for(int i = 0; i < width*height*4; i++) { //Loop to set pixel values to the Image Magick Image
            int row = get_row(i, width*2, height*2); //get row
            int col = get_column(i, width*2, height*2); //get col
            size_t offset = (width*2 * row + col); //calculate offset in array
            size_t mainOffset = newImage.channels() * offset; //calculate final pixel index/offset
            newImagePixels[mainOffset + 0] = get_red(newPixels[i]); //set channels
            newImagePixels[mainOffset + 1] = get_green(newPixels[i]);
            newImagePixels[mainOffset + 2] = get_blue(newPixels[i]);
            newImagePixels[mainOffset + 3] = get_alpha(newPixels[i]);
          }
          newImage.write(destination); //write image
          cout << "Done.\n";
          break;
        }
        case 3: { //Blur
          int blurLevel; //display blur choices
          cout << "==== Blur ====\n";
          cout << "1. Weak\n";
          cout << "2. Medium\n";
          cout << "3. Strong\n";
          cout << "Select blur level: ";
          cin >> blurLevel;
          string maskPath; //store mask path
          if(blurLevel == 1) {
            maskPath = "masks/blur-slightly-mask.txt";
          } else if(blurLevel == 2) {
            maskPath = "masks/blur-mask.txt";
          } else {
            maskPath = "masks/blur-more-mask.txt";
          }
          ifstream maskFile(maskPath); //get mask file to read from
          int maskWidth;
          string temp;
          getline(maskFile, temp);
          maskWidth = stoi(temp);
          int* mask = (int*)calloc(sizeof(int), maskWidth*maskWidth); //store mask
          int i = 0; //for index in mask array
          while(getline(maskFile, temp)) {
            mask[i] = stoi(temp);
            i++;
          }
          string destination; //store destination
          cout << "Enter destination file/path, nothing for default(imgs/blur.png): ";
          cin.ignore();
          getline(cin, destination);
          if(destination == "") {
            destination = "imgs/blur.png"; //default path
          }
          cout << "Writing image to " + destination + "..." << flush; //display progress
          pixel_t* newPixels = blur(pixels, width, height, mask, maskWidth); //store result in new pixel array
          string dimensions = to_string(width) + "x" + to_string(height); //create dimension string
          Image newImage((Geometry)dimensions, "transparent");
          Quantum *newImagePixels = newImage.getPixels(0, 0, width, height);
          for(int i = 0; i < width*height; i++) {
            int row = get_row(i, width, height);
            int col = get_column(i, width, height);
            size_t offset = (width * row + col);
            size_t mainOffset = newImage.channels() * offset;
            newImagePixels[mainOffset + 0] = get_red(newPixels[i]);
            newImagePixels[mainOffset + 1] = get_green(newPixels[i]);
            newImagePixels[mainOffset + 2] = get_blue(newPixels[i]);
            newImagePixels[mainOffset + 3] = get_alpha(newPixels[i]);
          }
          newImage.write(destination);
          free(mask);
          cout << "Done.\n";
          break;
        }
        case 4: { //Adjust contrast
          int contrastLevel;
          cout << "Enter contrast level: ";
          cin >> contrastLevel;
          string destination; //store destination
          cout << "Enter destination file/path, nothing for default(imgs/contrast.png): ";
          cin.ignore();
          getline(cin, destination);
          if(destination == "") {
            destination = "imgs/contrast.png"; //default path
          }
          cout << "Writing image to " + destination + "..." << flush; //display progress
          pixel_t* newPixels = contrast(pixels, width, height, contrastLevel); //store result in new pixel array
          string dimensions = to_string(width) + "x" + to_string(height); //create dimension string
          Image newImage((Geometry)dimensions, "transparent");
          Quantum *newImagePixels = newImage.getPixels(0, 0, width, height);
          for(int i = 0; i < width*height; i++) {
            int row = get_row(i, width, height);
            int col = get_column(i, width, height);
            size_t offset = (width * row + col);
            size_t mainOffset = newImage.channels() * offset;
            newImagePixels[mainOffset + 0] = get_red(newPixels[i]);
            newImagePixels[mainOffset + 1] = get_green(newPixels[i]);
            newImagePixels[mainOffset + 2] = get_blue(newPixels[i]);
            newImagePixels[mainOffset + 3] = get_alpha(newPixels[i]);
          }
          newImage.write(destination);
          free(newPixels);
          cout << "Done.\n";
          break;
        }
        case 5: { //Brightness
          int brightnessLevel;
          cout << "Enter brightness level: ";
          cin >> brightnessLevel;
          string destination; //store destination
          cout << "Enter destination file/path, nothing for default(imgs/brightness.png): ";
          cin.ignore();
          getline(cin, destination);
          if(destination == "") {
            destination = "imgs/brightness.png"; //default path
          }
          cout << "Writing image to " + destination + "..." << flush; //display progress
          pixel_t* newPixels = brightness(pixels, width, height, brightnessLevel); //store result in new pixel array
          string dimensions = to_string(width) + "x" + to_string(height); //create dimension string
          Image newImage((Geometry)dimensions, "transparent");
          Quantum *newImagePixels = newImage.getPixels(0, 0, width, height);
          for(int i = 0; i < width*height; i++) {
            int row = get_row(i, width, height);
            int col = get_column(i, width, height);
            size_t offset = (width * row + col);
            size_t mainOffset = newImage.channels() * offset;
            newImagePixels[mainOffset + 0] = get_red(newPixels[i]);
            newImagePixels[mainOffset + 1] = get_green(newPixels[i]);
            newImagePixels[mainOffset + 2] = get_blue(newPixels[i]);
            newImagePixels[mainOffset + 3] = get_alpha(newPixels[i]);
          }
          newImage.write(destination); //write to output
          free(newPixels);
          cout << "Done.\n";
          break;
        }
        case 6: { //Edge Detection (Sobel)
          string destination; //store destination
          string input;
          bool cropEdge = false;
          cout << "Enter destination file/path, nothing for default(imgs/edge.png): ";
          cin.ignore();
          getline(cin, destination);
          if(destination == "") {
            destination = "imgs/edge.png"; //default path
          }
          cout << "Crop edge(y/n)? ";
          cin >> input;
          if(input == "y" || input == "Y" || input == "yes" || input == "Yes") cropEdge = true;
          cout << "Writing image to " + destination + "..." << flush; //display progress
          Image grayScaleIMG(sourceImage); //Create new image copy for grayscale image
          grayScaleIMG.type(GrayscaleType); //Convert image from rgb to grayscale(1 channel)
          Quantum *graySourcePixels = grayScaleIMG.getPixels(0, 0, width, height); //get pixel data
          int* grayPixels = (int*)calloc(sizeof(int), width*height); //create grayscale pixel type
          //convert from image magick data to imageutil data(easier to work with)
          for(int i = 0; i < width*height; i++) {
            int row = get_row(i, width, height);
            int col = get_column(i, width, height);
            size_t offset = (width * row + col);
            size_t mainOffset = grayScaleIMG.channels() * offset;
            grayPixels[i] = graySourcePixels[mainOffset + 0]; //create our pixel and store it(only 1 channel)
          }
          //Create sobel masks
          int* maskX = (int*)calloc(sizeof(int), 9);
          maskX[0] = 1; maskX[1] = 0; maskX[2] = -1;
          maskX[3] = 2; maskX[4] = 0; maskX[5] = -2;
          maskX[6] = 1; maskX[7] = 0; maskX[8] = -1;
          int* maskY = (int*)calloc(sizeof(int), 9);
          maskY[0] = 1; maskY[1] = 2; maskY[2] = 1; 
          maskY[3] = 0; maskY[4] = 0; maskY[5] = 0; 
          maskY[6] = -1; maskY[7] = -2; maskY[8] = -1;
          pixel_t* newPixels = edge(grayPixels, width, height, maskX, maskY, 3); //store result in new pixel array
          string dimensions = to_string(width) + "x" + to_string(height); //create dimension string
          Image newImage((Geometry)dimensions, "white"); 
          newImage.type(GrayscaleType);
          Quantum *newImagePixels = newImage.getPixels(0, 0, width, height);
          for(int i = 0; i < width*height; i++) {
            int row = get_row(i, width, height);
            int col = get_column(i, width, height);
            size_t offset = (width * row + col);
            size_t mainOffset = newImage.channels() * offset;
            newImagePixels[mainOffset + 0] = (newPixels[i]);
          }
          if(cropEdge) { //Crop edge if choosen
            newImage.crop(Geometry(width-2,height-2,1,1));
          }
          newImage.write(destination); //write to output
          free(maskX); //free masks
          free(maskY);
          free(newPixels);
          cout << "Done.\n";
          break;
        }
        case 7: { //Stamp Art
          int cutoffLevel;
          cout << "Enter cutoff level(0-255): ";
          cin >> cutoffLevel;
          string destination; //store destination
          cout << "Enter destination file/path, nothing for default(imgs/art.png): ";
          cin.ignore();
          getline(cin, destination);
          if(destination == "") {
            destination = "imgs/art.png"; //default path
          }
          cout << "Writing image to " + destination + "..." << flush; //display progress
          Image grayScaleIMG(sourceImage); //Create new image copy for grayscale image
          grayScaleIMG.type(GrayscaleType); //Convert image from rgb to grayscale(1 channel)
          Quantum *graySourcePixels = grayScaleIMG.getPixels(0, 0, width, height); //get pixel data
          int* grayPixels = (int*)calloc(sizeof(int), width*height); //create grayscale pixel type
          //convert from image magick data to imageutil data(easier to work with)
          for(int i = 0; i < width*height; i++) {
            int row = get_row(i, width, height);
            int col = get_column(i, width, height);
            size_t offset = (width * row + col);
            size_t mainOffset = grayScaleIMG.channels() * offset;
            grayPixels[i] = graySourcePixels[mainOffset + 0]; //create our pixel and store it(only 1 channel)
          }
          pixel_t* newPixels = art(grayPixels, width, height, cutoffLevel);
          string dimensions = to_string(width*2) + "x" + to_string(height*2); //create dimension string
          Image newImage((Geometry)dimensions, "transparent"); //set to 4 channels
          Quantum *newImagePixels = newImage.getPixels(0, 0, width*2, height*2); //Same copy algorithm as berfore
          for(int i = 0; i < width*height*4; i++) {
            int row = get_row(i, width*2, height*2);
            int col = get_column(i, width*2, height*2);
            size_t offset = (width*2 * row + col);
            size_t mainOffset = newImage.channels() * offset;
            newImagePixels[mainOffset + 0] = get_red(newPixels[i]);
            newImagePixels[mainOffset + 1] = get_green(newPixels[i]);
            newImagePixels[mainOffset + 2] = get_blue(newPixels[i]);
            newImagePixels[mainOffset + 3] = get_alpha(newPixels[i]);
          }
          newImage.write(destination); //write to output
          free(newPixels);
          cout << "Done.\n";
          break;
        }
        case 8: { //Sharpen
          int strength;
          int threshold;
          cout << "Enter the strength(amount): ";
          cin >> strength;
          cout << "Enter the threshold: ";
          cin >> threshold;
          string destination; //store destination
          cout << "Enter destination file/path, nothing for default(imgs/sharpen.png): ";
          cin.ignore();
          getline(cin, destination);
          if(destination == "") {
            destination = "imgs/sharpen.png"; //default path
          }
          cout << "Writing image to " + destination + "..." << flush; //display progress
          /* GET MASK FOR BLUR */
          string maskPath = "masks/blur-more-mask.txt";
          ifstream maskFile(maskPath); //get mask file to read from
          int maskWidth;
          string temp;
          getline(maskFile, temp);
          maskWidth = stoi(temp);
          int* mask = (int*)calloc(sizeof(int), maskWidth*maskWidth); //store mask
          int i = 0; //for index in mask array
          while(getline(maskFile, temp)) {
            mask[i] = stoi(temp);
            i++;
          }
          pixel_t* newPixels = sharpen(pixels, width, height, strength, threshold, mask);
          string dimensions = to_string(width) + "x" + to_string(height); //create dimension string
          Image newImage((Geometry)dimensions, "transparent");
          Quantum *newImagePixels = newImage.getPixels(0, 0, width, height);
          for(int i = 0; i < width*height; i++) {
            int row = get_row(i, width, height);
            int col = get_column(i, width, height);
            size_t offset = (width * row + col);
            size_t mainOffset = newImage.channels() * offset;
            newImagePixels[mainOffset + 0] = get_red(newPixels[i]);
            newImagePixels[mainOffset + 1] = get_green(newPixels[i]);
            newImagePixels[mainOffset + 2] = get_blue(newPixels[i]);
            newImagePixels[mainOffset + 3] = get_alpha(newPixels[i]);
          }
          newImage.write(destination); //write to output
          free(mask);
          free(newPixels);
          cout << "Done.\n";
          break;
        }
	      case 9: { //Oil Painting
          int intensityLevel;
          int maskWidth;
          cout << "Enter intensity level: ";
          cin >> intensityLevel;
          cout << "Enter mask/matrix width: ";
          cin >> maskWidth;
          string destination; //store destination
          cout << "Enter destination file/path, nothing for default(imgs/oil.png): ";
          cin.ignore();
          getline(cin, destination);
          if(destination == "") {
            destination = "imgs/oil.png"; //default path
          }
          cout << "Writing image to " + destination + "..." << flush; //display progress
          pixel_t* newPixels = oil(pixels, width, height, maskWidth, intensityLevel);
          string dimensions = to_string(width) + "x" + to_string(height); //create dimension string
          Image newImage((Geometry)dimensions, "transparent");
          Quantum *newImagePixels = newImage.getPixels(0, 0, width, height);
          for(int i = 0; i < width*height; i++) { //Copy to new image BLOB
            int row = get_row(i, width, height);
            int col = get_column(i, width, height);
            size_t offset = (width * row + col);
            size_t mainOffset = newImage.channels() * offset;
            newImagePixels[mainOffset + 0] = get_red(newPixels[i]);
            newImagePixels[mainOffset + 1] = get_green(newPixels[i]);
            newImagePixels[mainOffset + 2] = get_blue(newPixels[i]);
            newImagePixels[mainOffset + 3] = get_alpha(newPixels[i]);
          }
          newImage.write(destination); //write to output
          free(newPixels);
          cout << "Done.\n";
          break;
        }
        case 10: { //Emboss
          break;
        }
        case 11: { //Segmentation
          break;
        }
        case 12: { //Quit
	        break;
	      }
        default: {
          cout << "Invalid selection. Please try again.\n";
          break;
        }
      }
    } while(choice != 12);
    cout << "Goodbye.\n";
  }  
  catch ( Magick::Exception & error) { 
    cerr << "\nCaught Magick++ exception: " << error.what() << endl;
  }
  return 0;
}

