#ifndef _CANNY_SEQUENTIAL_HPP_
#define _CANNY_SEQUENTIAL_HPP_
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "timer.h"

#define KERNEL_SIZE 7

//__device__ typedef std::vector<double> Array;
//__device__ typedef std::vector<Array> Matrix;


void edge_detector(unsigned char* h_newImg, unsigned char* h_imgbuff, int WIDTH, int HEIGHT);


/*create a gaussian filter*/
double** createKernel(int height, int width, double sigma);
/*Step 1 blur the image to reduce noice*/
void gaussian_filter(unsigned char**newImage, unsigned char **in_pixels,int width, int height);
void gradient(unsigned char **newImage, unsigned char **mag, int width, int height,
                         unsigned char **deltaX, unsigned char **deltaY);
void apply_hysteresis(unsigned char **out_pixels, unsigned char **in_pixels, unsigned  char t_high, unsigned  char t_low, int width,int height);
void trace_immed_neighbors(unsigned char **out_pixels, unsigned char **in_pixels, unsigned i, unsigned j, unsigned  char t_low);

#endif
