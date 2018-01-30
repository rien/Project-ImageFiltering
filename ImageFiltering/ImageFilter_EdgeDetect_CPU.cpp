#include "FilterUtils.h"
#include "ImageFilter_EdgeDetect_CPU.h"

Image* ImageFilter_EdgeDetect_CPU::Filter(Image* sourceImageOne, Image* sourceImageTwo)
{
	auto edgedetect = [=](unsigned int* tile_r, unsigned int* tile_g, unsigned int* tile_b, unsigned int* tile_a, int x, int y, int gridSize) restrict(amp, cpu)
	{
		unsigned int rx = 0, ry = 0, gx = 0, gy = 0, bx = 0, by = 0;

		unsigned int sobel[CONVSIZE] = { 1, 2, 1 };

		for (int i = 0; i < CONVSIZE; ++i)
		{
			int delta = i - CONVSIZE / 2;
			int posx = x + 1 + (y + delta)*gridSize;
			int posy = x + delta + (y - 1)*gridSize;

			rx += sobel[i] * tile_r[posx];
			gx += sobel[i] * tile_g[posx];
			bx += sobel[i] * tile_b[posx];
			ry += sobel[i] * tile_r[posy];
			gy += sobel[i] * tile_g[posy];
			by += sobel[i] * tile_b[posy];
		}
		for (int i = 0; i < CONVSIZE; ++i)
		{
			int delta = i - CONVSIZE / 2;
			int posx = x - 1 + (y + delta)*gridSize;
			int posy = x + delta + (y + 1)*gridSize;

			unsigned int subrx = sobel[i] * tile_r[posx];
			unsigned int subgx = sobel[i] * tile_g[posx];
			unsigned int subbx = sobel[i] * tile_b[posx];
			unsigned int subry = sobel[i] * tile_r[posy];
			unsigned int subgy = sobel[i] * tile_g[posy];
			unsigned int subby = sobel[i] * tile_b[posy];

			// Prevent underflow
			rx -= rx < subrx ? rx : subrx;
			gx -= gx < subgx ? gx : subgx;
			bx -= bx < subbx ? bx : subbx;
			ry -= ry < subry ? ry : subry;
			gy -= gy < subgy ? gy : subgy;
			by -= by < subby ? by : subby;

		}
		// Prevent overflow
		rx = rx > 255 ? 255 : rx;
		gx = gx > 255 ? 255 : gx;
		bx = bx > 255 ? 255 : bx;
		ry = ry > 255 ? 255 : ry;
		gy = gy > 255 ? 255 : gy;
		by = by > 255 ? 255 : by;


		unsigned int r = edgeIntensity(rx, ry);
		unsigned int g = edgeIntensity(gx, gy);
		unsigned int b = edgeIntensity(bx, by);

		return toPixel(r, g, b, tile_a[x + y*gridSize]);
	};
	return process_tile_CPU(sourceImageOne, edgedetect);
}