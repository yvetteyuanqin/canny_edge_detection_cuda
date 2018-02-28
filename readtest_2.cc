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


//void printPixel()


int main() {
	
    using namespace boost::gil;
    gray8_image_t img;
    png_read_image("001.png", img);
    png_write_view("testimg.png",const_view(img));
    

   // image view
    image_view<gray8_view_t> img_view(img);

    
    for (int i = 0; i < img_view.height(); i++) {
		gray8_view_t::x_iterator it = img_view.row_begin(i);
		for (int j = 0; j < sizeof(it); j++) cout << it[j];
        
        cout << endl;
    }

    //
	return 0;
}
