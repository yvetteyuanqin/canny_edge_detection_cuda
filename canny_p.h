#ifndef _CANNY_SEQUENTIAL_HPP_
#define _CANNY_SEQUENTIAL_HPP_
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "timer.h"

#define KERNEL_SIZE 7

typedef std::vector<double> Array;
typedef std::vector<Array> Matrix;


void edge_detector(unsigned char** h_newImg, unsigned char** h_imgbuff, int WIDTH, int HEIGHT);


/*create a gaussian filter*/
__device__ Matrix createKernel(int height, int width, double sigma);
/*Step 1 blur the image to reduce noice*/
__global__ void gaussian_filter(unsigned char**newImage, unsigned char **in_pixels,int width, int height);
__global__ void gradient(unsigned char **newImage, unsigned char **mag, int width, int height,
	unsigned char **deltaX, unsigned char **deltaY);
__global__ void suppress(unsigned char **newImage, unsigned char **mag, int width, int height,
	unsigned char **deltaX, unsigned char **deltaY);
__global__ void apply_hysteresis(unsigned char **out_pixels, unsigned char **in_pixels, unsigned  char t_high, unsigned  char t_low, int width,int height);
__global__ void trace_immed_neighbors(unsigned char **out_pixels, unsigned char **in_pixels, unsigned i, unsigned j, unsigned  char t_low);

#endif
