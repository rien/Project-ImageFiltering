#pragma once
#include "ImageFilter.h"

class ImageFilter_Median_GPU: public ImageFilter
{
public:
	ImageFilter_Median_GPU(){};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_Median_GPU(){};
};

