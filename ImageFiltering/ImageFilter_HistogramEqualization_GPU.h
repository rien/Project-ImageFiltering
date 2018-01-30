#pragma once
#include <ImageFilter.h>

class ImageFilter_HistogramEqualization_GPU: public ImageFilter
{
public:
	ImageFilter_HistogramEqualization_GPU(){};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	~ImageFilter_HistogramEqualization_GPU(){};
};

