#include "ImageFilter_Invert_CPU.h"
#include <Image.h>
#include "FilterUtils.h"

Image* ImageFilter_Invert_CPU::Filter(Image* sourceImageOne, Image* sourceImageTwo) {
	auto invert = [](unsigned int& r, unsigned int& g, unsigned int& b, unsigned int& a) restrict(amp,cpu) {
		r = 255 - r;
		g = 255 - g;
		b = 255 - b;
	};
	return process_per_pixel_CPU(sourceImageOne, invert);
}

ImageFilter_Invert_CPU::ImageFilter_Invert_CPU()
{
}

ImageFilter_Invert_CPU::~ImageFilter_Invert_CPU()
{
}
