#include "ImageFilter_HistogramEqualization_CPU.h"
#include <thread>
#include <atomic>
#include "WorkBalance.h"
#include "PixelUtils.h"

Image* ImageFilter_HistogramEqualization_CPU::Filter(Image* sourceImageOne, Image* sourceImageTwo)
{
	std::atomic_uint vFreq[256] = { 0 };
	unsigned int cumFreq[256] = { 0 };
	float values[256] = { 0.0 };
	int width = sourceImageOne->GetWidth();
	int heigth = sourceImageOne->GetHeight();
	unsigned int* newData = new unsigned int[width*heigth];
	unsigned int* oldData = reinterpret_cast<unsigned*>(sourceImageOne->GetPixelData());

	unsigned int aThreads = std::thread::hardware_concurrency();
	aThreads = aThreads == 0 ? 1 : aThreads;
	WorkBalance allPixels(width*heigth, aThreads);
	WorkBalance valueBalance(256, aThreads);

    // Tel de voorkomens van de verschillende v-waardes.
	allPixels.executeParallel([&](int from, int to) {
		unsigned r, g, b, a;
		float h, s, v;
		for (int i = from; i < to; ++i)
		{
			getColorData(oldData[i], r, g, b, a);
			RGBtoHSV(r, g, b, h, s, v);
			int index = int(v * 255);
			vFreq[index] += 1;
		}
	});

    // Bepaal de cumulatieve som en de minimale van 0 verschillende waarde.
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
	
    // Bereken de nieuwe v-waardes.
	float div = float((width*heigth - cdfMin));
	valueBalance.executeParallel([&](int from, int to)
	{
		for (int i = from; i < to; ++i)
		{
			values[i] = float((cumFreq[i] - cdfMin)) / div;
		}
	});

    // Vervang de v-waardes van alle pixels.
	allPixels.executeParallel([&](int from, int to)
	{
		unsigned r, g, b, a;
		float h, s, v;
		for (int i = from; i < to; ++i)
		{
			getColorData(oldData[i], r, g, b, a);
			RGBtoHSV(r, g, b, h, s, v);
			v = values[int(v * 255)];
			HSVtoRGB(h, s, v, r, g, b);
			newData[i] = toPixel(r, g, b, a);
		}
	});
	return new Image(reinterpret_cast<unsigned char*>(newData), width, heigth);
}
