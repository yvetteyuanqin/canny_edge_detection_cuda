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
using namespace std;

int main() {
	
    using namespace boost::gil;
    gray8_image_t img;
    png_read_image("001.png", img);
    png_write_view("testimg.png",const_view(img));



	Image_view img_view(img);
	for (int i = 0; i < img_view.height(); i++) {
		for (int j = 0; j < img_view.width(); j++) {
			cout << img_view.xy_at(j, i);
		}
		cout << endl;
	}



}
