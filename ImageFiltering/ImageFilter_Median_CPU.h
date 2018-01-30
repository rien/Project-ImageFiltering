#pragma once
#include "ImageFilter.h"

class ImageFilter_Median_CPU : public ImageFilter
{
public:
	ImageFilter_Median_CPU() {};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_Median_CPU() {};
};

