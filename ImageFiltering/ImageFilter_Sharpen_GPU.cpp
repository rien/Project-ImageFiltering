#include "ImageFilter_Sharpen_GPU.h"
#include "FilterUtils.h"

Image* ImageFilter_Sharpen_GPU::Filter(Image* sourceImageOne, Image* sourceImageTwo)
{
	auto sharpen = [=](unsigned int* tile_r, unsigned int* tile_g, unsigned int* tile_b, unsigned int* tile_a, int x, int y, int gridSize) restrict(amp, cpu)
	{
		unsigned int r, g, b;

		r = tile_r[x + y*gridSize] * 5;
		g = tile_g[x + y*gridSize] * 5;
		b = tile_b[x + y*gridSize] * 5;

		//Relative pairs of coordinates of the -1's
		int cords[8] = { 0, -1, 0, 1, -1, 0, 1, 0 };
		unsigned int subr, subg, subb;
		for (int i = 0; i < 4; ++i)
		{
			int pos = x + cords[i * 2] + (y + cords[i * 2 + 1])*gridSize;
			subr = tile_r[pos];
			subg = tile_g[pos];
			subb = tile_b[pos];

			// Prevent underflow
			r -= (subr > r ? r : subr);
			g -= (subg > g ? g : subg);
			b -= (subb > b ? b : subb);
		}

		// Prevent overflow
		r = r > 255 ? 255 : r;
		g = g > 255 ? 255 : g;
		b = b > 255 ? 255 : b;


		return toPixel(r, g, b, tile_a[x + y*gridSize]);
	};
	return process_tile_GPU(sourceImageOne, sharpen);
}
