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
void apply_hysteresis(boost::gil::gray8_pixel_t **out_pixels, boost::gil::gray8_pixel_t **in_pixels, char t_high, char t_low, int width,int height);
void trace_immed_neighbors(boost::gil::gray8_pixel_t **out_pixels, boost::gil::gray8_pixel_t **in_pixels, unsigned i, unsigned j, char t_low);

#endif
