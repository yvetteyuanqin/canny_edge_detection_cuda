/**
 *  Sequential canny detection
 *
 *
 */
//Yuan Qin
//Lai Man Tang

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

#include <boost/gil/rgb.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <stdint.h>
#include <vector>

struct PixelInserter{
    std::vector<uint8_t>* storage;
    PixelInserter(std::vector<uint8_t>* s) : storage(s) {}
    void operator()(boost::gil::rgb8_pixel_t p) const {
        storage->push_back(boost::gil::at_c<0>(p));
//        storage->push_back(boost::gil::at_c<1>(p));
//        storage->push_back(boost::gil::at_c<2>(p));
    }
};

int main() {
    std::vector<uint8_t> storage;
    {
        using namespace boost::gil;
        rgb8_image_t img;
        png_read_image("001.png", img);
        storage.reserve(img.width() * img.height());//* num_channels<rgb8_image_t>());
        for_each_pixel(const_view(img), PixelInserter(&storage));
    }
    
}
