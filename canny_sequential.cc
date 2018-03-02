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
    int newImageHeight = height-filterHeight+1;
    int newImageWidth = width-filterWidth+1;

	cout << "newImageHeight " << newImageHeight << " newImageWidth " << newImageWidth << endl;
	cout << "filterHeight " << filterHeight  << " filterWidth " << filterWidth << endl;
	cout << "width " << width << " height " << height << endl;

    int i,j,h,w;
    /*allocate newimage*/
    
	cout << "start convolution" << endl;
        for (i=0 ; i<newImageHeight ; i++) {
			cout << i << " " << endl;
            for (j=0 ; j<newImageWidth ; j++) {
                for (h=i ; h<i+filterHeight ; h++) {
                    for (w=j ; w<j+filterWidth ; w++) {
                        newImage[i][j] = newImage[i][j] +filter[h-i][w-j]*in_pixels[h][w];
                    }
                }
            }
        }

}


