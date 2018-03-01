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
#define HEIGHT 512
#define WIDTH 512

using namespace std;

int main() {
    
    using namespace boost::gil;
    gray8_image_t img(HEIGHT,WIDTH);
    png_read_image("001.png", img);
    png_write_view("testimg.png",const_view(img));
    
    
    
    // Get a raw pointer to the grey buffer
    unsigned char * buf = &view(img)[0][0];//new unsigned char[w * h];

    gray8_view_t srcview = interleaved_view(img.width(), img.height(), (gray8_pixel_t*)buf, view(img).pixels().row_size());
    int cnt = 0;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            buff[i][j]=img_view(j, i);
        }
    }
//    for (size_t i = 0; i < img.height(); ++i)
//    {
//        boost::gil::gray8_view_t::x_iterator it = srcview.row_begin(i);
//        for (size_t j = 0; j<img.width(); ++j){
//            //boost::gil::gray8_view_t::y_iterator jt = srcview.col_begin(j);
//            cout << hex << int(it[j]) ;
//            cnt++;
//        }
//        // use it[j] to access pixel[i][j]
//    }
    cout<<endl;
    cout<<cnt<<endl;
    cout<<img.width()<<' '<<img.height()<<endl;
   
    
    return 0;
    
}

