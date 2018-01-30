#pragma once
#include <ImageFilter.h>

class ImageFilter_HueShift_GPU: public ImageFilter
{
public:
	ImageFilter_HueShift_GPU(){};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_HueShift_GPU(){};
};

