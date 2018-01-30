#pragma once
#include <Image.h>
#include <amp.h>
#include <cassert>

#include "PixelUtils.h"
#include "WorkBalance.h"
#define TILESIZE 8
#define GRIDSIZE 10
#define RADIUS 2
#define CONVSIZE 3

using namespace Concurrency;

template<typename Functor>
Image* process_per_pixel_GPU(Image* source, Functor pixelManipulator)
{
	int width = source->GetWidth();
	int heigth = source->GetHeight();
	unsigned int* newData = new unsigned int[width*heigth];

	array_view<const unsigned int, 2> src(width, heigth, reinterpret_cast<const unsigned int*>(source->GetPixelData()));
	array_view<unsigned int, 2> dest(width, heigth, reinterpret_cast<unsigned int*>(newData));
	dest.discard_data();

	parallel_for_each(dest.extent, [=](index<2> index) restrict(amp)
	{
		unsigned int pixel = src(index);
		unsigned int r, g, b, a;
		getColorData(pixel, r, g, b, a);
		pixelManipulator(r, g, b, a);
		dest(index) = toPixel(r, g, b, a);
	});
	return new Image(reinterpret_cast<unsigned char*>(newData), width, heigth);
}

template<typename Functor>
Image* process_per_pixel_CPU(Image* source, Functor pixelManipulator)
{
	int width = source->GetWidth();
	int heigth = source->GetHeight();
	unsigned int* oldData = reinterpret_cast<unsigned int*>(source->GetPixelData());
	unsigned int* newData = new unsigned int[width*heigth];


	int aThreads = std::thread::hardware_concurrency();
	aThreads = aThreads == 0 ? 1 : aThreads;

	auto processPixels = [&](int from, int to) {
		for (int i = from; i < to; i++)
		{
			unsigned int pixel = oldData[i];
			unsigned int r, g, b, a;
			getColorData(pixel, r, g, b, a);
			pixelManipulator(r, g, b, a);
			newData[i] = toPixel(r, g, b, a);
		}
	};

	WorkBalance balance(width*heigth, aThreads);
	balance.executeParallel(processPixels);
	return new Image(reinterpret_cast<unsigned char*>(newData), width, heigth);
}

template<typename Functor>
Image* process_tile_GPU(Image* source, Functor tileManipulator)
{
	int width = source->GetWidth();
	int heigth = source->GetHeight();
	unsigned int* newData = new unsigned int[width*heigth];
	array_view<const unsigned int, 2> src(heigth, width, reinterpret_cast<const unsigned int*>(source->GetPixelData()));
	array_view<unsigned int, 2> dest(heigth, width, reinterpret_cast<unsigned int*>(newData));

	dest.discard_data();

	WorkBalance balance(GRIDSIZE*GRIDSIZE, TILESIZE*TILESIZE);
	array_view<const int> kernelStart(TILESIZE*TILESIZE, balance.threadStart);
	array_view<const int> kernelEnd(TILESIZE*TILESIZE, balance.threadEnd);


	parallel_for_each(src.extent.tile<TILESIZE, TILESIZE>(), [=](tiled_index<TILESIZE, TILESIZE> idx) restrict(amp)
	{
		tile_static unsigned int tile_r[GRIDSIZE*GRIDSIZE];
		tile_static unsigned int tile_g[GRIDSIZE*GRIDSIZE];
		tile_static unsigned int tile_b[GRIDSIZE*GRIDSIZE];
		tile_static unsigned int tile_a[GRIDSIZE*GRIDSIZE];

		int local_x = idx.local[0],
			local_y = idx.local[1],
			origin_x = idx.tile_origin[0],
			origin_y = idx.tile_origin[1];

		int kernel_nr = (local_x + local_y*TILESIZE);
		
		for (int i = kernelStart[kernel_nr]; i < kernelEnd[kernel_nr]; i++)
		{
			int grid_x = i % GRIDSIZE;
			int grid_y = i / GRIDSIZE;

			int actual_x = origin_x + grid_x - RADIUS/2;
			int actual_y = origin_y + grid_y - RADIUS/2;

			if (actual_x < 0) {
				actual_x = 0;
			} else if (heigth <= actual_x) {
				actual_x = heigth - 1;
			}
			if (actual_y < 0) {
				actual_y = 0;
			} else if (width <= actual_y) {
				actual_y = width - 1;
			}
			unsigned int r, g, b, a;
			getColorData(src(actual_x, actual_y), r, g, b, a);
			tile_r[grid_x + grid_y*GRIDSIZE] = r;
			tile_g[grid_x + grid_y*GRIDSIZE] = g;
			tile_b[grid_x + grid_y*GRIDSIZE] = b;
			tile_a[grid_x + grid_y*GRIDSIZE] = a;
		}
		idx.barrier.wait_with_tile_static_memory_fence();
		dest(idx.global) = tileManipulator(tile_r, tile_g, tile_b, tile_a, local_x + RADIUS/2, local_y + RADIUS/2, GRIDSIZE);
	});

	return new Image(reinterpret_cast<unsigned char*>(newData), width, heigth);
}

template<typename Functor>
Image* process_tile_CPU(Image* source, Functor tileManipulator)
{
	int width = source->GetWidth();
	int heigth = source->GetHeight();
	unsigned int* oldData = reinterpret_cast<unsigned int*>(source->GetPixelData());
	unsigned int* newData = new unsigned int[width*heigth];

	int paddedWidth = (width + RADIUS);
	int paddedSize = paddedWidth*(heigth + RADIUS);
	unsigned int* rData = new unsigned int[paddedSize];
	unsigned int* gData = new unsigned int[paddedSize];
	unsigned int* bData = new unsigned int[paddedSize];
	unsigned int* aData = new unsigned int[paddedSize];


	unsigned int aThreads = std::thread::hardware_concurrency();
	aThreads = aThreads == 0 ? 1 : aThreads;

	int offset = RADIUS / 2;

	WorkBalance balanceExtract(paddedSize, aThreads);
	// Extract color
	balanceExtract.executeParallel([&](int from, int to) {
		for (int i = from; i < to; ++i)
		{
			unsigned int r, g, b, a;
			int padded_x = (i % paddedWidth);
			int padded_y = (i / paddedWidth);
			int actual_x = padded_x - offset;
			int actual_y = padded_y - offset;
			if (actual_x < 0) {
				actual_x = 0;
			}
			else if (width <= actual_x) {
				actual_x = width - 1;
			}
			if (actual_y < 0) {
				actual_y = 0;
			}
			else if (heigth <= actual_y) {
				actual_y = heigth - 1;
			}
			getColorData(oldData[actual_x + actual_y*width], r, g, b, a);
			rData[i] = r;
			gData[i] = g;
			bData[i] = b;
			aData[i] = a;
		}
	});

	WorkBalance balanceProcess(width*heigth, aThreads);
	// Process the pixels
	balanceProcess.executeParallel([&](int from, int to) {
		for (int i = from; i < to; ++i)
		{
			int x = i % width;
			int y = i / width;
			newData[i] = tileManipulator(rData, gData, bData, aData, x + offset, y + offset, paddedWidth);
		}
	});
	delete[] rData;
	delete[] gData;
	delete[] bData;
	delete[] aData;
	return new Image(reinterpret_cast<unsigned char*>(newData), width, heigth);
}
