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
void gradient(gray8_pixel_t **newImage, gray8_pixel_t **in_pixels, int width, int height,
	gray8_pixel_t **deltaX, gray8_pixel_t **deltaY)
{


	   // compute delta X ***************************
	   // deltaX = f(x+1) - f(x-1)
	cout << "do deltaX" << endl;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++){
			if (j == 0) deltaX[i][j] = in_pixels[i][j + 1] - in_pixels[i][j];
			else if (j == width - 1)deltaX[i][j] = in_pixels[i][j] - in_pixels[i][j - 1];
			else deltaX[i][j] = in_pixels[i][j + 1] - in_pixels[i][j - 1];
		}
	}
	cout << "finished deltaX" << endl;
	cout << "do deltaY" << endl;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (i == 0) deltaY[i][j] = in_pixels[i+1][j] - in_pixels[i][j];
			else if (i == height - 1)deltaY[i][j] = in_pixels[i][j] - in_pixels[i-1][j];
			else deltaY[i][j] = in_pixels[i+1][j] - in_pixels[i-1][j];
		}
	}
	cout << "finished deltaY" << endl;
	cout << "do magnitude" << endl;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
				newImage[i][j] = (gray8_pixel_t)(sqrt((double)deltaX[i][j] * deltaX[i][j] +
					(double)deltaY[i][j] * deltaY[i][j]) + 0.5);
		}
	}
	cout << "finish " << endl;

}

void suppress(gray8_pixel_t **newImage, gray8_pixel_t **mag, int width, int height,
	gray8_pixel_t **deltaX, gray8_pixel_t **deltaY)
{
    
    float alpha;
    float mag1, mag2;
    const pixel_channel_t SUPPRESSED = 0;
    
    // put zero all boundaries of image
    // TOP edge line of the image
	for (int k = 0; k < height; ++k) {
		newImage[height - 1][k] = 0;
		newImage[0][k] = 0;
		newImage[k][0] = 0;
		newImage[k][width - 1]= 0;
	}
   

    // skip boundaries of image
    // start and stop 1 pixel inner pixels from boundaries
    for(unsigned i = 1; i < height-1; i++)
    {
        for(unsigned j = 1; j < width-1; j++)
        {
            // if magnitude = 0, no edge
            if(mag[i][j] == 0) newImage[i][j] = 0;//suppressed
            else{
                if(deltaX[i][j] >= 0)
                {
                    if(deltaY[i][j] >= 0)  // dx >= 0, dy >= 0
                    {
                        if((deltaX[i][j] - deltaY[i][j]) >= 0)       // direction 1 (SEE, South-East-East)
                        {
                            alpha = (float)deltaY[i][j] / deltaX[i][j]
                            mag1 = (1-alpha)*mag[i][j+1] + alpha*mag[i+1][j+1];
                            mag2 = (1-alpha)*mag[i][j-1] + alpha*mag[i-1][j-1];
                        }
                        else                                // direction 2 (SSE)
                        {
                            alpha = (float)deltaX[i][j] / deltaY[i][j]
                            mag1 = (1-alpha)*mag[i+1][j] + alpha*mag[i+1][j+1];
                            mag2 = (1-alpha)*mag[i-1][j] + alpha*mag[i-1][j-1];
                            
                        }
                    }
                    
                    else  // dx >= 0, dy < 0
                    {
                        if((deltaX[i][j] + deltaY[i][j]) >= 0)    // direction 8 (NEE)
                        {
                            alpha = (float)-deltaY[i][j] / deltaX[i][j]
                            mag1 = (1-alpha)*mag[i][j+1] + alpha*mag[i-1][j+1];
                            mag2 = (1-alpha)*mag[i][j-1] + alpha*mag[i+1][j-1];
                            
                        }
                        else                                // direction 7 (NNE)
                        {
                            
                            alpha = (float)deltaX[i][j] / -deltaY[i][j]
                            mag1 = (1-alpha)*mag[i+1][j] + alpha*mag[i+1][j-1];
                            mag2 = (1-alpha)*mag[i-1][j] + alpha*mag[i-1][j+1];
                            
                        }
                    }
                }
                
                else
                {
                    if(deltaY[i][j] >= 0) // dx < 0, dy >= 0
                    {
                        if((deltaX[i][j] - deltaY[i][j]) >= 0)    // direction 3 (SSW)
                        {
                            alpha = (float)-deltaX[i][j] / deltaY[i][j]
                            mag1 = (1-alpha)*mag[i+1][j] + alpha*mag[i+1][j-1];
                            mag2 = (1-alpha)*mag[i-1][j] + alpha*mag[i-1][j+1];
                        }
                        else                                // direction 4 (SWW)
                        {
                            alpha = (float)deltaY[i][j] / -deltaX[i][j]
                            mag1 = (1-alpha)*mag[i][j-1] + alpha*mag[i+1][j-1];
                            mag2 = (1-alpha)*mag[i][j+1] + alpha*mag[i-1][j+1];
                        }
                    }
                    
                    else // dx < 0, dy < 0
                    {
                        if((-deltaX[i][j] + deltaY[i][j]) >= 0)   // direction 5 (NWW)
                        {
                            alpha = (float)deltaY[i][j] / deltaX[i][j]
                            mag1 = (1-alpha)*mag[i][j-1] + alpha*mag[i-1][j-1];
                            mag2 = (1-alpha)*mag[i][j+1] + alpha*mag[i+1][j+1];
                        }
                        else                                // direction 6 (NNW)
                        {
                            alpha = (float)deltaX[i][j] / deltaY[i][j]
                            mag1 = (1-alpha)*mag[i-1][j] + alpha*mag[i-1][j-1];
                            mag2 = (1-alpha)*mag[i+1][j] + alpha*mag[i+1][j+1];
                        }
                    }
                }
                
                // non-maximal suppression
                // compare mag1, mag2 and mag[t]
                // if mag[t] is smaller than one of the neighbours then suppress it
                if((mag[i][j] < mag1) || (mag[i][j] < mag2))
					newImage[i][j] = 0;//SUPRRESSED
                else
                {
					newImage[i][j] = mag[i][j];
                }
                
            }
        }
    }
}




