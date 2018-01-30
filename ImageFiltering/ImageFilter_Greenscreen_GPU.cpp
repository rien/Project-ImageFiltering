#include "ImageFilter_Greenscreen_GPU.h"
#include "PixelUtils.h"
#include "FilterUtils.h"

Image* ImageFilter_Greenscreen_GPU::Filter(Image* sourceImageOne, Image* sourceImageTwo) {
	unsigned int rkey = this->rkey, gkey = this->gkey, bkey = this->bkey, dMin = this->dMin, dMax = this->dMax;
	auto greenScreen = [=](unsigned int& r, unsigned int& g, unsigned int& b, unsigned int& a) restrict(amp, cpu)
	{
		unsigned int dMins = dMin*dMin, dMaxs = dMax*dMax;
		unsigned int dist = colorDistance(r, g, b, rkey, gkey, bkey);
		if (dist <= dMin)
		{
			a = 0;
		}
		else if (dist < dMax)
		{

			a = ((dist - dMin)*a) / (dMax - dMin);
		}
	};
	return process_per_pixel_GPU(sourceImageOne, greenScreen);
}
