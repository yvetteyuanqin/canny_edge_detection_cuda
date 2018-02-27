/**
 *  Sequential canny detection
 *
 *
 */
//Yuan Qin
//Lai Man Tang
#include <boost/mpl/vector.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>

namespace gil = boost::gil;
namespace mpl= boost::mpl;
using namespace gil;
using namespace mpl;

typedef mpl::vector<gray8_image_t, gray16_image_t, rgb8_image_t, rgb16_image_t> my_img_types;

int main() {
	any_image<my_img_types> runtime_image;
	png_read_image("001.png", runtime_image);

	gray8s_image_t gradient(runtime_image.dimensions());
	x_luminosity_gradient(const_view(runtime_image), view(gradient));
	png_write_view("testoutput.png", color_converted_view<gray8_pixel_t>(const_view(gradient)));

	return 0;
}