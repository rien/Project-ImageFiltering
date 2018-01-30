#include "ImageFilter_Invert_GPU.h"
#include "FilterUtils.h"

Image* ImageFilter_Invert_GPU::Filter(Image* sourceImageOne, Image* sourceImageTwo) {
	auto invert = [](unsigned int& r, unsigned int& g, unsigned int& b, unsigned int& a) restrict(amp,cpu) {
		r = 255 - r;
		g = 255 - g;
		b = 255 - b;
	};
	return process_per_pixel_GPU(sourceImageOne, invert);
}

ImageFilter_Invert_GPU::ImageFilter_Invert_GPU()
{
}


ImageFilter_Invert_GPU::~ImageFilter_Invert_GPU()
{
}
