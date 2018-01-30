#pragma once
#include "ImageFilter.h"
class ImageFilter_GaussianBlurFilter_GPU :
	public ImageFilter
{
public:
	ImageFilter_GaussianBlurFilter_GPU(){};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_GaussianBlurFilter_GPU(){};
};

