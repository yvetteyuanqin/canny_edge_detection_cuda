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
    
    
    
    
    unsigned char * buf = new unsigned char[w * h];
    
    gray8_view_t image = interleaved_view(w, h, (gray8_pixel_t*)buf, w);
    
    for (size_t i = 0; i < ...; ++i)
    {
        boost::gil::gray8_view_t::x_iterator it = image.row_begin(i);
        
        // use it[j] to access pixel[i][j]
    }
    
    
    //    for (int i = 0; i < img_view.height(); i++) {
    //        for (int j = 0; j < img_view.width(); j++) {
    //            cout << img_view;
    //        }
    //        cout << endl;
    //    }
    
    
    
}

