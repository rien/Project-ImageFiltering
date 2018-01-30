#pragma once
#include "ImageFilter.h"
class ImageFilter_Desaturate_CPU :
	public ImageFilter
{
public:
	ImageFilter_Desaturate_CPU();
	virtual ~ImageFilter_Desaturate_CPU();
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo);
};

