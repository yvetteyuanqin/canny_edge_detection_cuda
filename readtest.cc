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


int main() {
	
    using namespace boost::gil;
    gray8_image_t img;
    png_read_image("001.png", img);
    png_write_view("testimg.png",const_view(img));
    for(int i=0;i<img.height();i++){
        for(int j =0;j<img.width();j++)
            cout<<img[i][j];
        cout<<endl;
    }
    

}
