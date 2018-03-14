/**
*  Sequential canny detection
*
*
*/
//Yuan Qin
//Lai Man Tang

//Mar.13th.2018
#include <boost/gil/rgb.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <boost/fusion/view.hpp>
#include <boost/fusion/include/view.hpp>
#include <stdint.h>
#include <vector>
#include <iostream>
#include "canny_p.h"
#include "timer.h"
#define HEIGHT 512
#define WIDTH 512

using namespace std;

int main() {
	/* initialize timer */
   
    struct stopwatch_t* timer = NULL;
    long double t_gaussian, t_gradient, t_nms, t_thres;
    stopwatch_init();
    timer = stopwatch_create();

	//string filename = argv[1]
	using namespace boost::gil;
	gray8_image_t img(HEIGHT, WIDTH);
	png_read_image("001.png", img);
	//png_write_view("testimg.png",const_view(img));


	// Get a raw pointer to the gray buffer
	unsigned char * buf = &view(img)[0][0];//new unsigned char[w * h];
	unsigned char **h_imgbuff = (unsigned char**)malloc(sizeof(unsigned char*)*WIDTH);
	for (int i = 0; i < WIDTH; i++)
	{
		*(h_imgbuff + i) = (unsigned char*)malloc(sizeof(unsigned char)*HEIGHT);
	}

	/*put the image pixel into the gray buffer*/
	gray8_view_t srcview = interleaved_view(img.width(), img.height(), (gray8_pixel_t*)buf, view(img).pixels().row_size());

	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			h_imgbuff[i][j] = (unsigned char)srcview(j, i);
		}
	}

	/*canny_sequential*/
	unsigned char **h_newImage = (unsigned char**)malloc(sizeof(unsigned char*)*HEIGHT);
	for (int i = 0; i < WIDTH; i++)
	{
		*(h_newImage + i) = (unsigned char*)malloc(sizeof(unsigned char)*WIDTH);
		for (int j = 0; j < HEIGHT; j++) h_newImage[i][j] = 0;
	}
    
	for (int i = 0; i < HEIGHT; i++) {
	//	cout << i << endl;
		for (int j = 0; j < WIDTH; j++) {
			cout << h_newImage[i][j] << " ";
		}
		cout << endl;
	}
    stopwatch_start(timer);
    edge_detector(h_newImage, h_imgbuff, WIDTH, HEIGHT);
    t_gaussian = stopwatch_stop(timer);
    cout<< "Time to execute gaussian:"<< t_gaussian<<endl;
    


   
	gray8_pixel_t **newImage = (gray8_pixel_t**)malloc(sizeof(gray8_pixel_t*)*HEIGHT);
	for (int i = 0; i < WIDTH; i++)
	{
		*(newImage + i) = (gray8_pixel_t*)malloc(sizeof(gray8_pixel_t)*WIDTH);
		for (int j = 0; j<HEIGHT; j++) newImage[i][j] = h_newImage[i][j];
	}

	cout << "transfer to gray8" << endl;


	/*revert image to check*/
	gray8_image_t img2(HEIGHT, WIDTH);
	gray8_view_t img_view = view(img2);

	cout << "create image view" << endl;
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			img_view(j, i) = newImage[i][j];
		}
	}
	cout << "finished." << endl;

	png_write_view("testimg.png", const_view(img2));


	return 0;

}
