
/* pixel, header file
 * C++, 2018
 * Alan Chen
 * 
 * 
 * Provides pixel data structure and functions
 */

typedef int pixel;  
typedef pixel pixel_t;

int get_red(pixel p);
int get_green(pixel p);
int get_blue(pixel p);
int get_alpha(pixel p);
pixel make_pixel(int alpha, int red, int green, int blue);
