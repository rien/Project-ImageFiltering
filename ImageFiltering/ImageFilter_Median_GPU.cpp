#include "ImageFilter_Median_GPU.h"
#include "FilterUtils.h"


Image* ImageFilter_Median_GPU::Filter(Image* sourceImageOne, Image* sourceImageTwo)
{
	auto median = [=](unsigned int* tile_r, unsigned int* tile_g, unsigned int* tile_b, unsigned int* tile_a, int x, int y, int gridSize) restrict(amp, cpu)
	{
		unsigned int dr[9], dg[9], db[9];
		for(int i = 0; i < 9; ++i)
		{
			int dx = (i % CONVSIZE) - (CONVSIZE / 2);
			int dy = (i / CONVSIZE) - (CONVSIZE / 2);
			int pos = x + dx + (y + dy)*gridSize;
			dr[i] = tile_r[pos];
			dg[i] = tile_g[pos];
			db[i] = tile_b[pos];
		}
		sort9(dr);
		sort9(dg);
		sort9(db);
		return toPixel(dr[5], dg[5], db[5], tile_a[x + y*gridSize]);
	};
	return process_tile_GPU(sourceImageOne, median);
}
