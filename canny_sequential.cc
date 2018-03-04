#include <boost/gil/rgb.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "canny_sequential.h"


using namespace boost::gil;
using namespace std;


/*create a gaussian filter*/
Matrix createKernel(int height, int width, double sigma)
{
    Matrix kernel(height, Array(width));
    double sum=0.0;
    int i,j;
    
    for (i=0 ; i<height ; i++) {
        for (j=0 ; j<width ; j++) {
            kernel[i][j] = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
            sum += kernel[i][j];
        }
    }
    
    for (i=0 ; i<height ; i++) {
        for (j=0 ; j<width ; j++) {
            kernel[i][j] /= sum;
        }
    }
    
    return kernel;
}

/*Step 1 blur the image to reduce noice*/
void gaussian_filter(gray8_pixel_t **newImage,gray8_pixel_t **in_pixels,int width, int height)
{

	cout << "create kernel" << endl;
    Matrix filter = createKernel(5, 5, 10.0);
	cout << "finish create kernel" << endl;
    int filterHeight = filter.size();
    int filterWidth = filter[0].size();
    int newImageHeight = height-filterHeight;
    int newImageWidth = width-filterWidth;

	cout << "newImageHeight " << newImageHeight << " newImageWidth " << newImageWidth << endl;
	cout << "filterHeight " << filterHeight  << " filterWidth " << filterWidth << endl;
	cout << "width " << width << " height " << height << endl;

    int i,j,h,w;
    /*allocate newimage*/
    
	cout << "start convolution" << endl;
        for (i=0 ; i<newImageHeight ; i++) {
            for (j=0 ; j<newImageWidth ; j++) {
                for (h=i ; h<i+filterHeight ; h++) {
                    for (w=j ; w<j+filterWidth ; w++) {
                        newImage[i][j] = newImage[i][j] +filter[h-i][w-j]*in_pixels[h][w];
                    }
                }
            }
        }

}
void gradient(gray8_pixel_t **newImage, gray8_pixel_t **in_pixels, int width, int height)
{

	gray8_pixel_t **deltaX = (gray8_pixel_t**)malloc(sizeof(gray8_pixel_t*)*height);
	gray8_pixel_t **deltaY = (gray8_pixel_t**)malloc(sizeof(gray8_pixel_t*)*height);

	for (int i = 0; i < width; i++)
	{
		*(deltaX + i) = (gray8_pixel_t*)malloc(sizeof(gray8_pixel_t)*width);
		*(deltaY + i) = (gray8_pixel_t*)malloc(sizeof(gray8_pixel_t)*width);
	}


	//   gray8_pixel_t **deltaX = (gray8_pixel_t)new pixel_t_signed[max_pixel_cnt];
	  // pixel_t_signed *deltaY = new pixel_t_signed[max_pixel_cnt];

	   // compute delta X ***************************
	   // deltaX = f(x+1) - f(x-1)
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++){
			if (j == 0) deltaX[i][j] = in_pixels[i][j + 1] - in_pixels[i][j];
			else if (j == width - 1)deltaX[i][j] = in_pixels[i][j] - in_pixels[i][j - 1];
			else deltaX[i][j] = in_pixels[i][j + 1] - in_pixels[i][j - 1];
		}
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (j == 0) deltaY[i][j] = in_pixels[i+1][j] - in_pixels[i][j];
			else if (j == width - 1)deltaY[i][j] = in_pixels[i][j] - in_pixels[i-1][j];
			else deltaY[i][j] = in_pixels[i+1][j] - in_pixels[i-1][j];
		}
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
				newImage[i][j] = (gray8_pixel_t)(sqrt((double)deltaX[i][j] * deltaX[i][j] +
					(double)deltaY[i][j] * deltaY[i][j]) + 0.5);
		}
	}



}

