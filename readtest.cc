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
//#include "interleaved_ptr.hpp"
using namespace std;

int main() {
    
    using namespace boost::gil;
    gray8_image_t img;
    png_read_image("001.png", img);
    png_write_view("testimg.png",const_view(img));
    
    
    
    
    unsigned char * buf = &view(img)[0][0];//new unsigned char[w * h];

    gray8_view_t srcview = interleaved_view(img.width(), img.height(), (gray8_pixel_t*)buf, view(img).pixels().row_size());
    
    for (size_t i = 0; i < img.height(); ++i)
    {
        boost::gil::gray8_view_t::x_iterator it = srcview.row_begin(i);
        for (size_t j = 0; j<img.width(); ++j){
            boost::gil::gray8_view_t::y_iterator jt = srcview.col_begin(j);
            cout<< it[j] ;
        }
        // use it[j] to access pixel[i][j]
    }
    
    
    //    for (int i = 0; i < img_view.height(); i++) {
    //        for (int j = 0; j < img_view.width(); j++) {
    //            cout << img_view;
    //        }
    //        cout << endl;
    //    }
    
    return 0;
    
}

