#ifndef _CANNY_SEQUENTIAL_HPP_
#define _CANNY_SEQUENTIAL_HPP_
#include <boost/gil/rgb.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <vector>
#define KERNEL_SIZE 7

typedef std::vector<double> Array;
typedef std::vector<Array> Matrix;

/*create a gaussian filter*/
Matrix createKernel(int height, int width, double sigma);
/*Step 1 blur the image to reduce noice*/
void gaussian_filter(boost::gil::gray8_pixel_t **newImage,boost::gil::gray8_pixel_t **in_pixels,int width, int height);
void gradient(boost::gil::gray8_pixel_t **newImage, boost::gil::gray8_pixel_t **mag, int width, int height,
	boost::gil::gray8_pixel_t **deltaX, boost::gil::gray8_pixel_t **deltaY);
void suppress(boost::gil::gray8_pixel_t **newImage, boost::gil::gray8_pixel_t **mag, int width, int height,
	boost::gil::gray8_pixel_t **deltaX, boost::gil::gray8_pixel_t **deltaY);


#endif
