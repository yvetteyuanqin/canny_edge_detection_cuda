#ifndef _CANNY_SEQUENTIAL_HPP_
#define _CANNY_SEQUENTIAL_HPP_
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
void gaussian_filter(unsigned char**newImage, unsigned char **in_pixels,int width, int height);
void gradient(unsigned char **newImage, unsigned char **mag, int width, int height,
	unsigned char **deltaX, unsigned char **deltaY);
void suppress(unsigned char **newImage, unsigned char **mag, int width, int height,
	unsigned char **deltaX, unsigned char **deltaY);
void apply_hysteresis(unsigned char **out_pixels, unsigned char **in_pixels, unsigned  char t_high, unsigned  char t_low, int width,int height);
void trace_immed_neighbors(unsigned char **out_pixels, unsigned char **in_pixels, unsigned i, unsigned j, unsigned  char t_low);

void edge_detector((unsigned char)** h_newImg, (unsigned char)** h_imgbuff, int WIDTH, int HEIGHT);
#endif
