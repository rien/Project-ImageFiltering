#pragma once
#include <ImageFilter.h>

class ImageFilter_Greenscreen_CPU: public ImageFilter
{
	unsigned int rkey, gkey, bkey, dMax, dMin;
public:
	ImageFilter_Greenscreen_CPU() : rkey(52), gkey(220), bkey(77), dMax(120), dMin(90) {};
	ImageFilter_Greenscreen_CPU(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _dMax, unsigned int _dMin) :
		rkey(_r), gkey(_g), bkey(_b), dMax(_dMax), dMin(_dMin) {};
	Image* Filter(Image* sourceImageOne, Image* sourceImageTwo) override;
	virtual ~ImageFilter_Greenscreen_CPU() {};
};

