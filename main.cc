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
#include "timer.h"
#define HEIGHT 512
#define WIDTH 512

using namespace std;

int main() {
    
    struct stopwatch_t* timer = NULL;
    long double t_gaussian, t_gradient,t_nms,t_thres;
    /* initialize timer */
    stopwatch_init ();
    timer = stopwatch_create ();
    stopwatch_start (timer);
//    t_gaussian= stopwatch_stop (timer);
//    cout<< "Time to execute gaussian:"<< t_gaussian<<endl;
    
    
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
    t_gaussian= stopwatch_stop (timer);
    gaussian_filter(newImage,imgbuff,WIDTH, HEIGHT);
    cout<< "Time to execute gaussian:"<< t_gaussian<<endl;
	cout << "finished." << endl;
    

	gray8_pixel_t **gradientImg = (gray8_pixel_t**)malloc(sizeof(gray8_pixel_t*)*HEIGHT);
	for (int i = 0; i < WIDTH; i++)
	{
		*(gradientImg + i) = (gray8_pixel_t*)malloc(sizeof(gray8_pixel_t)*WIDTH);
		for (int j = 0; j<HEIGHT; j++) gradientImg[i][j] = 0;
	}

    gray8_pixel_t **deltaX = (gray8_pixel_t**)malloc(sizeof(gray8_pixel_t*)*HEIGHT);
    gray8_pixel_t **deltaY = (gray8_pixel_t**)malloc(sizeof(gray8_pixel_t*)*HEIGHT);
    
    for (int i = 0; i < WIDTH; i++)
    {
        *(deltaX + i) = (gray8_pixel_t*)malloc(sizeof(gray8_pixel_t)*WIDTH);
        *(deltaY + i) = (gray8_pixel_t*)malloc(sizeof(gray8_pixel_t)*WIDTH);
    }
    
	/*Gradient*/
	cout << "enter gradient filter" << endl;
    t_gradient= stopwatch_stop (timer);
    gradient(gradientImg, newImage, WIDTH, HEIGHT,deltaX,deltaY);
    cout<< "Time to execute gradient:"<< t_gradient<<endl;
	
	cout << "finished." << endl;
    
    
    gray8_pixel_t **NMSImg = (gray8_pixel_t**)malloc(sizeof(gray8_pixel_t*)*HEIGHT);
    for (int i = 0; i < WIDTH; i++)
    {
        *(NMSImg + i) = (gray8_pixel_t*)malloc(sizeof(gray8_pixel_t)*WIDTH);
        for (int j = 0; j<HEIGHT; j++) NMSImg[i][j] = 0;
    }
    
    /*non maximum suppression*/
    cout << "enter suppression filter" << endl;
    t_nms= stopwatch_stop (timer);
    suppress(NMSImg,gradientImg,WIDTH, HEIGHT,deltaX,deltaY);
    cout<< "Time to execute nms:"<< t_nms<<endl;
    
    cout << "finished." << endl;

    
    gray8_pixel_t **thresImg = (gray8_pixel_t**)malloc(sizeof(gray8_pixel_t*)*HEIGHT);
    for (int i = 0; i < WIDTH; i++)
    {
        *(thresImg + i) = (gray8_pixel_t*)malloc(sizeof(gray8_pixel_t)*WIDTH);
        for (int j = 0; j<HEIGHT; j++) thresImg[i][j] = 0;
    }
    
    /*non hysterious thresholding*/
    cout << "enter hysterious" << endl;
	unsigned char hi = 0xFC;
	unsigned char lo = 0xC0;
    t_thres= stopwatch_stop (timer);
    apply_hysteresis(thresImg,NMSImg, hi, lo, WIDTH,HEIGHT);
    cout<< "Time to execute thresholded:"<< t_thres<<endl;

    
    cout << "finished." << endl;
    
    
    /*revert image to check*/
    gray8_image_t img2(HEIGHT,WIDTH);
    auto img_view = view(img2);
    
	cout << "create image view" << endl;
	for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            img_view(j, i) = thresImg[i][j];
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

