#include "ImageFilter_Desaturate_CPU.h"
#include "FilterUtils.h"

Image* ImageFilter_Desaturate_CPU::Filter(Image* sourceImageOne, Image* sourceImageTwo) {
	auto desaturate = [=](unsigned int& r, unsigned int& g, unsigned int& b, unsigned int& a) restrict(amp,cpu)
	{
		unsigned int median = (r + g + b) / 3;
		r = median;
		g = median;
		b = median;
	};
	return process_per_pixel_CPU(sourceImageOne, desaturate);
}

ImageFilter_Desaturate_CPU::ImageFilter_Desaturate_CPU()
{
}


ImageFilter_Desaturate_CPU::~ImageFilter_Desaturate_CPU()
{
}

