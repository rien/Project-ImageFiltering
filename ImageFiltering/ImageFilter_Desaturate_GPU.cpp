#include "ImageFilter_Desaturate_GPU.h"
#include "FilterUtils.h"

ImageFilter_Desaturate_GPU::~ImageFilter_Desaturate_GPU()
{
}

Image* ImageFilter_Desaturate_GPU::Filter(Image* sourceImageOne, Image* sourceImageTwo){
	auto desaturate = [=](unsigned int& r, unsigned int& g, unsigned int& b, unsigned int& a) restrict(amp, cpu)
	{
		unsigned int median = (r + g + b) / 3;
		r = median;
		g = median;
		b = median;
	};
	return process_per_pixel_GPU(sourceImageOne, desaturate);
}

