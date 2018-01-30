#pragma once
#include "ImageFilter.h"
class ImageFilter_EdgeDetect_CPU : public ImageFilter
{
public:
	ImageFilter_EdgeDetect_CPU() {};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_EdgeDetect_CPU() {};
};

