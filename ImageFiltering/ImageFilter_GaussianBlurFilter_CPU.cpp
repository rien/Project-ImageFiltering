#include "ImageFilter_GaussianBlurFilter_CPU.h"
#include "FilterUtils.h"
#include "PixelUtils.h"

Image* ImageFilter_GaussianBlurFilter_CPU::Filter(Image* sourceImageOne, Image* sourceImageTwo)
{
	auto gaussianBlur = [=](unsigned int* tile_r, unsigned int* tile_g, unsigned int* tile_b, unsigned int* tile_a, int x, int y, int gridSize) restrict(amp, cpu)
	{
		const unsigned int matrix[CONVSIZE*CONVSIZE] = { 1, 2, 1,
			2, 4, 2,
			1, 2, 1 };

		unsigned int r = 0, g = 0, b = 0, a = 0;
		for (int i = 0; i < CONVSIZE*CONVSIZE; i++) {
			int dx = (i % CONVSIZE) - (CONVSIZE / 2);
			int dy = (i / CONVSIZE) - (CONVSIZE / 2);
			int pos = x + dx + (y + dy)*gridSize;
			r += tile_r[pos] * matrix[i];
			g += tile_g[pos] * matrix[i];
			b += tile_b[pos] * matrix[i];
		}
		r >>= 4;
		g >>= 4;
		b >>= 4;
		return toPixel(r, g, b, tile_a[x + y*gridSize]);
	};
	return process_tile_GPU(sourceImageOne, gaussianBlur);
}
