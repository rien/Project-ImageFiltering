#pragma once
#include "ImageFilter.h"
class ImageFilter_GaussianBlurFilter_CPU :
	public ImageFilter
{
public:
	ImageFilter_GaussianBlurFilter_CPU(){};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_GaussianBlurFilter_CPU(){};
};

