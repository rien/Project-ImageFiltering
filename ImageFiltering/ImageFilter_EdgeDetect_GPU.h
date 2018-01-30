#pragma once
#include "ImageFilter.h"
class ImageFilter_EdgeDetect_GPU: public ImageFilter
{
public:
	ImageFilter_EdgeDetect_GPU(){};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_EdgeDetect_GPU(){};
};

