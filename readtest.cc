/**
 *  Sequential canny detection
 *
 *
 */
//Yuan Qin
//Lai Man Tang

#include <boost/mpl/vector.hpp>


typedef mpl::vector<gray8_image_t, gray16_image_t, rgb8_image_t, rgb16_image_t> my_img_types;
any_image<my_img_types> runtime_image;
png_read_image("001.png", runtime_image);

gray8s_image_t gradient(runtime_image.dimensions());
x_luminosity_gradient(const_view(runtime_image), view(gradient));
png_write_view("testoutput.png", color_converted_view<gray8_pixel_t>(const_view(gradient)));
