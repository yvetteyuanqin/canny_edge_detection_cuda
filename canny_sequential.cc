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
void gradient(gray8_pixel_t **newImage,gray8_pixel_t **in_pixels,int width, int height)
{
    
    
    
    
        pixel_t_signed *deltaX = new pixel_t_signed[max_pixel_cnt];
        pixel_t_signed *deltaY = new pixel_t_signed[max_pixel_cnt];

        // compute delta X ***************************
        // deltaX = f(x+1) - f(x-1)
        for(unsigned i = 0; i < parser_length; ++i)
        {
            idx = offset * i; // current position X per line
            
            // gradient at the first pixel of each line
            // note: the edge,pix[idx-1] is NOT exsit
            deltaX[idx].red = (int16_t)(in_pixels[idx+1].red - in_pixels[idx].red);
            
            
            // gradients where NOT edge
            for(unsigned j = 1; j < offset-1; ++j)
            {
                idx++;
                deltaX[idx].red = (int16_t)(in_pixels[idx+1].red - in_pixels[idx-1].red);
                
            }
            
            // gradient at the last pixel of each line
            idx++;
            deltaX[idx].red = (int16_t)(in_pixels[idx].red - in_pixels[idx-1].red);

        }
        
        // compute delta Y ***************************
        // deltaY = f(y+1) - f(y-1)
        for(unsigned j = 0; j < offset; ++j)
        {
            idx = j;    // current Y position per column
            // gradient at the first pixel
            deltaY[idx].red = (int16_t)(in_pixels[idx+offset].red - in_pixels[idx].red);

            
            // gradients for NOT edge pixels
            for(unsigned i = 1; i < parser_length-1; ++i)
            {
                idx += offset;
                deltaY[idx].red = (int16_t)(in_pixels[idx+offset].red - in_pixels[idx-offset].red);
    
            }
            
            // gradient at the last pixel of each column
            idx += offset;
            deltaY[idx].red = (int16_t)(in_pixels[idx].red - in_pixels[idx-offset].red);

        }
        
    

}

