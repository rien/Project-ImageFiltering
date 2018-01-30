#pragma once
#include "ImageFilter.h"
class ImageFilter_Sharpen_GPU: public ImageFilter
{
public:
	ImageFilter_Sharpen_GPU(){};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_Sharpen_GPU(){};
};

