#pragma once
#include "ImageFilter.h"

class ImageFilter_Invert_CPU :
	public ImageFilter
{
public:
	ImageFilter_Invert_CPU();
	Image* ImageFilter_Invert_CPU::Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_Invert_CPU();
};

