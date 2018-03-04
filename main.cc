/**
 *  Sequential canny detection
 *
 *
 */
//Yuan Qin
//Lai Man Tang


#include <boost/gil/rgb.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <stdint.h>
#include <vector>
#include <iostream>
#include "canny_sequential.h"
#define HEIGHT 512
#define WIDTH 512

using namespace std;

int main() {
    
	//string filename = argv[1]
    using namespace boost::gil;
    gray8_image_t img(HEIGHT,WIDTH);
    png_read_image("001.png", img);
    //png_write_view("testimg.png",const_view(img));
    
    
    
    // Get a raw pointer to the grey buffer
    unsigned char * buf = &view(img)[0][0];//new unsigned char[w * h];
    gray8_pixel_t **imgbuff = (gray8_pixel_t**)malloc(sizeof(gray8_pixel_t*)*WIDTH);
    for(int i = 0; i < WIDTH; i++)
    {
        *(imgbuff+i) = (gray8_pixel_t*)malloc(sizeof(gray8_pixel_t)*HEIGHT);
    }
    
    gray8_view_t srcview = interleaved_view(img.width(), img.height(), (gray8_pixel_t*)buf, view(img).pixels().row_size());
    int cnt = 0;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            imgbuff[i][j]=srcview(j, i);
           // cout<<hex<<int(imgbuff[i][j]);
           
        } cnt += 1;
    }

    /*canny_sequential*/
    gray8_pixel_t **newImage = (gray8_pixel_t**)malloc(sizeof(gray8_pixel_t*)*HEIGHT);
    for(int i = 0; i < WIDTH; i++)
    {
        *(newImage+i) = (gray8_pixel_t*)malloc(sizeof(gray8_pixel_t)*WIDTH);
        for(int j = 0;j<HEIGHT;j++) newImage[i][j]=0;
    }
    
     cout<<endl;
     cout<<cnt<<endl;
     cout<<sizeof(imgbuff)<<' '<<sizeof(imgbuff)<<endl;
     
    
    /*apply gaussian filter*/
	cout << "enter gaussian filter" << endl;
    gaussian_filter(newImage,imgbuff,WIDTH, HEIGHT);
	cout << "finished." << endl;
    

	gray8_pixel_t **gradientImg = (gray8_pixel_t**)malloc(sizeof(gray8_pixel_t*)*HEIGHT);
	for (int i = 0; i < WIDTH; i++)
	{
		*(gradientImg + i) = (gray8_pixel_t*)malloc(sizeof(gray8_pixel_t)*WIDTH);
		for (int j = 0; j<HEIGHT; j++) gradientImg[i][j] = 0;
	}


	/*Gradient*/
	cout << "enter gradient filter" << endl;
	gradient(gradientImg, newImage, WIDTH, HEIGHT);
	cout << "finished." << endl;


    /*revert image to check*/
    gray8_image_t img2(HEIGHT,WIDTH);
    auto img_view = view(img2);
    
	cout << "create image view" << endl;
	for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            img_view(j, i) = gradientImg[i][j];
        }
    }
	cout << "finished." << endl;
    
    png_write_view("testimg.png", const_view(img2));
   /*
    cout<<endl;
    cout<<cnt<<endl;
    cout<<sizeof(imgbuff)<<' '<<sizeof(imgbuff)<<endl;
*/
    
    return 0;
    
}

