#pragma once
#include <ImageFilter.h>

class ImageFilter_Invert_GPU: public ImageFilter{
public:
	ImageFilter_Invert_GPU();
	virtual ~ImageFilter_Invert_GPU();
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
};


