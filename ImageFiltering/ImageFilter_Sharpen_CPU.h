#pragma once
#include "ImageFilter.h"
class ImageFilter_Sharpen_CPU : public ImageFilter
{
public:
	ImageFilter_Sharpen_CPU() {};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_Sharpen_CPU() {};
};

