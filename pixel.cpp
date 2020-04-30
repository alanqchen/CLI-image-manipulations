
/* pixel, implementation file
 * C++, 2018
 * Alan Chen
 *
 * 
 * Provides pixel data structure and functions
 */

#include "pixel.h"

/* The pixel type and an interface to pixels */

typedef int pixel;       // Library (concrete) view of a pixel
typedef pixel pixel_t;   // Client  (abstract) view of a pixel

// Returns the red component of the given pixel p.
// POSTCONDITION: returned value is between 0 and 255, inclusive.
int get_red(pixel p)
//@ensures \result >= 0 && \result <= 255;
{
  return (p & 0x00FF0000) >> 16;
}

// Returns the green component of the given pixel p.
// POSTCONDITION: returned value is between 0 and 255, inclusive.
int get_green(pixel p)
//@ensures \result >= 0 && \result <= 255;
{
  return (p & 0x0000FF00) >> 8;
}

// Returns the blue component of the given pixel p.
// POSTCONDITION: returned value is between 0 and 255, inclusive.
int get_blue(pixel p)
//@ensures \result >= 0 && \result <= 255;
{
  return (p & 0x000000FF);
}

// Returns the alpha component of the given pixel p.
// POSTCONDITION: returned value is between 0 and 255, inclusive.
int get_alpha(pixel p)
//@ensures \result >= 0 && \result <= 255;
{
  return ((p & 0xFF000000) >> 24) & 0x000000FF;
}

// Returns an int representing an RGB pixel consisting of the given
// alpha, red, green and blue intensity values.
// PRECONDITION: all intensity values must be between 0 and 255,
// inclusive.
pixel make_pixel(int alpha, int red, int green, int blue)
//@requires alpha >= 0 && alpha <= 255 && red >= 0 && red <= 255;
//@requires green >= 0 && green <= 255 && blue >= 0 && blue <= 255;
{
  return (alpha << 24) | (red << 16) | (green << 8) | (blue);
}
