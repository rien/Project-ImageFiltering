#pragma once
#include <ImageFilter.h>

class ImageFilter_Desaturate_GPU: public ImageFilter
{
public:
	ImageFilter_Desaturate_GPU(){};
	virtual ~ImageFilter_Desaturate_GPU();
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo);
};

