#pragma once
#include <ImageFilter.h>
class Image;

class ImageFilter_HistogramEqualization_CPU: public ImageFilter
{
public:
	ImageFilter_HistogramEqualization_CPU(){};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_HistogramEqualization_CPU(){};
};

