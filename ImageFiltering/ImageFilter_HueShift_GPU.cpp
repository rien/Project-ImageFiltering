#include "ImageFilter_HueShift_GPU.h"
#include "PixelUtils.h"
#include "FilterUtils.h"


Image* ImageFilter_HueShift_GPU::Filter(Image* sourceImageOne, Image* sourceImageTwo) {
	auto invert = [](unsigned int& r, unsigned int& g, unsigned int& b, unsigned int& a) restrict(amp, cpu) {
		float h, s, v;
		RGBtoHSV(r, g, b, h, s, v);
		h -= 30;
		if (h < 0)
		{
			h += 360;
		}
		HSVtoRGB(h, s, v, r, g, b);
		return toPixel(r, g, b, a);
	};
	return process_per_pixel_GPU(sourceImageOne, invert);
}
