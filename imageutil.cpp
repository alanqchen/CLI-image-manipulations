#include <climits>
#include "imageutil.h"

// Checks that width and height are valid dimensions. To be valid
// dimensions, width and height must be strictly positive, and
// the product width*height must not cause overflow.
bool is_valid_imagesize(int width, int height)
{
  return width > 0
    && height > 0
    && width <= INT_MAX / height;
}

// Returns the row number of a pixel in an image of size width*
// height stored as a one-dimensional array given the index of the
// pixel in the one-dimensional array.
// PRECONDITION: width and height are valid dimensions and index is
// in-bounds for an array of length width*height.
// POSTCONDITION: returned value is between 0 (inclusive) and height
// (exclusive).
int get_row(int index, int width, int height)
//@requires is_valid_imagesize(width, height);
//@requires index >= 0 && index < width * height;
//@ensures \result >= 0 && \result < height;
{
  return (index - (index % width))/width;
}

// Returns the column number of a pixel in an image of size width*
// height stored as a one-dimensional array given the index of the
// pixel in the one-dimensional array.
// PRECONDITION: width and height are valid dimensions and index is
// in-bounds for an array of length width*height.
// POSTCONDITION: returned value is between 0 (inclusive) and width
// (exclusive).
int get_column(int index, int width, int height)
//@requires is_valid_imagesize(width, height);
//@requires index >= 0 && index < width * height;
//@ensures \result >= 0 && \result < width;
{
   return index % width;
}

// Given a width and height that are valid for an image, returns true
// if the given row and column represent a valid pixel location in an
// image of size width*height and false otherwise.
// PRECONDITION: width and height are valid dimensions
bool is_valid_pixel(int row, int col, int width, int height)
//@requires is_valid_imagesize(width, height);
{
  return row >= 0 && row < height && col >= 0 && col < width;
}

// Returns the index of a pixel in a one-dimensional array of pixels
// representing a two-dimensional image of size width*height, given
// the row and column of the pixel in the two-dimensional image.
// PRECONDITION: width and height are valid dimensions
// PRECONDITION: (row, col) is a valid pixel for an image of that
// width and height
// POSTCONDITION: returned value is in-bounds for an array of length
// width*height.
int get_index(int row, int col, int width, int height)
//@requires is_valid_imagesize(width, height);
//@requires is_valid_pixel(row, col, width, height);
//@ensures \result >= 0 && \result < width * height;
{
  return width * row + col;
}
