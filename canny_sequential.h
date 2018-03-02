#include <boost/gil/rgb.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <vector>
#define KERNEL_SIZE 7


typedef vector<double> Array;
typedef vector<Array> Matrix;

/*create a gaussian filter*/
Matrix createKernel(int height, int width, double sigma);
/*Step 1 blur the image to reduce noice*/
void gaussian_filter(gray8_pixel_t **newImage,gray8_pixel_t **in_pixels,int width, int height);


