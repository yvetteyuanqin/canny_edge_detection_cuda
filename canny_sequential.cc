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
//    filter[0][0]=1, filter[0][1]=4, filter[0][2]=7, filter[0][3]=4, filter[0][4]=1,
//    filter[1][0]=4, filter[1][1]=16, filter[1][2]=26, filter[1][3]=16, filter[1][4]=4,
//    filter[2][0]=7, filter[2][1]=26, filter[2][2]=41, filter[2][3]=26, filter[2][4]=7,
//    filter[3][0]=4, filter[3][1]=16, filter[3][2]=26, filter[3][3]=16, filter[3][4]=4,
//    filter[4][0]=1, filter[4][1]=4, filter[4][2]=7, filter[4][3]=4, filter[4][4]=1;
    int convolute=0;
    int x,y;
	cout << "start convolution" << endl;
        for (i=0 ; i<newImageHeight ; ++i) {
			
            for (j=0 ; j<newImageWidth ; ++j) {
				//cout << j << " ";
                x =i;
                y =j;
                for (h=0 ; h<filterHeight ; ++h) {
                    for (w=0 ; w<filterWidth ; ++w) {
                        convolute=convolute+filter[h][w]*in_pixels[x][y];
                        y++;
                    }
                    x++;
                    y=j;
                }
                newimage[i][j]=convolute;
                convolute = 0;
            }
        }

}


