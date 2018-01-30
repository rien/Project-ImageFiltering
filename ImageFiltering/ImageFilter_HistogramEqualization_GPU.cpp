#include "ImageFilter_HistogramEqualization_GPU.h"
#include <amp.h>
#include "PixelUtils.h"

using namespace Concurrency;

Image* ImageFilter_HistogramEqualization_GPU::Filter(Image* sourceImageOne, Image* sourceImageTwo)
{
	unsigned int vFreqArray[256] = {0};
	unsigned int cumFreqArray[256] = {0};
	float values[256] = {0.0};
	int width = sourceImageOne->GetWidth();
	int heigth = sourceImageOne->GetHeight();
	unsigned int* newData = new unsigned int[width*heigth];

	array_view<const unsigned int, 2> src(width, heigth, reinterpret_cast<const unsigned int*>(sourceImageOne->GetPixelData()));
	array_view<unsigned int> vFreq(256, vFreqArray);
	array_view<unsigned int> cumFreq(256
            , cumFreqArray);
	array_view<unsigned int, 2> dest(width, heigth, reinterpret_cast<unsigned int*>(newData));
	array_view<float> vals(256, values);
	dest.discard_data();

    // Tel de v-waardes van alle pixels.
	parallel_for_each(src.extent, [=](index<2> index) restrict(amp)
	{
		unsigned int pixel = src(index);
		unsigned int r, g, b, a;
		getColorData(pixel, r, g, b, a);
		float h, s, v;
		RGBtoHSV(r, g, b, h, s, v);
		atomic_fetch_inc(&vFreq(int(v * 255)));
	});

	// Bepaal de cummulatieve som en de minimale van 0 verchillende waarde
	unsigned int cdfMin = 0;
	cdfMin = vFreq[0];
	cumFreq[0] = vFreq[0];
	for (int i = 1; i < 256; ++i)
	{
		cumFreq[i] = vFreq[i] + cumFreq[i - 1];
		if (cdfMin == 0)
		{
			cdfMin = cumFreq[i];
		}
	}

    // Bereken voor iedere v-waarde zijn nieuwe waarde.
	float div = float((width*heigth - cdfMin));
	parallel_for_each(vals.extent, [=](index<1> index) restrict(amp)
	{
		vals(index) = float((cumFreq[index] - cdfMin)) / div;
	});

    // Vervang de v-waardes van alle pixels.
	parallel_for_each(dest.extent, [=](index<2> index) restrict(amp)
	{
		unsigned int pixel = src(index);
		unsigned int r, g, b, a;
		getColorData(pixel, r, g, b, a);
		float h, s, v;
		RGBtoHSV(r, g, b, h, s, v);
		v = vals(int(v*255));
		HSVtoRGB(h, s, v, r, g, b);
		dest(index) = toPixel(r, g, b, a);
	});
	return new Image(reinterpret_cast<unsigned char*>(newData), width, heigth);
}
