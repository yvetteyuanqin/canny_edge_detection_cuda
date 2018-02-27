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
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
namespace gil = boost::gil;
namespace mpl= boost::mpl;
using namespace gil;
using namespace mpl;



template <typename DstView>
struct x_gradient_obj {
    typedef void result_type;        // required typedef
    
    const DstView& _dst;
    x_gradient_obj(const DstView& dst) : _dst(dst) {}
    
    template <typename SrcView>
    void operator()(const SrcView& src) const { x_luminosity_gradient(src, _dst); }
};

template <typename SrcViews, typename DstView>
void x_luminosity_gradient(const any_image_view<SrcViews>& src, const DstView& dst) {
    apply_operation(src, x_gradient_obj<DstView>(dst));
}

typedef mpl::vector<gray8_image_t, gray16_image_t, rgb8_image_t, rgb16_image_t> my_img_types;

int main() {
	any_image<my_img_types> runtime_image;
	png_read_image("001.png", runtime_image);

	gray8s_image_t gradient(runtime_image.dimensions());
	x_luminosity_gradient(const_view(runtime_image), view(gradient));
	png_write_view("testoutput.png", color_converted_view<gray8_pixel_t>(const_view(gradient)));

	return 0;
}
