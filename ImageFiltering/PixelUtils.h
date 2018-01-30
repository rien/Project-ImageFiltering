#pragma once
#include "amp_math.h"

#define PIXEL_MASK 0x000000FF
#define NEXT(n, i)  (((n) + (i)/(n)) >> 1)
#define DIFF(x, y) ((x > y) ? (x - y) : (y - x))

inline void RGBtoHSV(const unsigned int& r,const unsigned int& g,const unsigned int& b, float& h, float& s, float& v) restrict(amp, cpu)
{
	enum color
	{
		R, G, B
	};
	float min, max, delta,
		rf = float(r)/255,
		gf = float(g)/ 255,
		bf = float(b)/ 255;
	unsigned int d[] = { r, g, b };

	#define SWAP(x,y) if (d[y] < d[x]) { unsigned int tmp = d[x]; d[x] = d[y]; d[y] = tmp; }
	SWAP(1, 2);
	SWAP(0, 2);
	SWAP(0, 1);
	#undef SWAP

	color maxColor = (d[2] == r) ? R : (d[2] == g ? G : B);
	max = float(d[2]) / 255;
	min = float(d[0]) / 255;
	delta = max - min;

	v = max;
	if(max != 0)
	{
		s = delta / max;
		switch (maxColor)
		{
		case R:
			h = (gf - bf)/ delta;
			break;
		case G:
			h = 2 + (bf - rf) / delta;
			break;
		case B:
			h = 4 + (rf - gf) / delta;
		}
		h *= 60;
		if(h < 0)
		{
			h += 360;
		}
	} else
	{
		s = 0;
		h = 0;
	}
}

inline void HSVtoRGB(const float& h, const float& s, const float& v, unsigned int& r, unsigned int& g, unsigned int& b) restrict(amp, cpu)
{
	int i;
	float e, f, p, q, t;
	if (s == 0) {
		// achromatic (grey)
		r = g = b = unsigned int(v*255);
		return;
	}
	e = h / 60;			// sector 0 to 5
	i = int(Concurrency::fast_math::floor(e));
	f = e - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	switch (i) {
	case 0:
		r = unsigned int(v * 255);
		g = unsigned int(t * 255);
		b = unsigned int(p * 255);
		break;
	case 1:
		r = unsigned int(q * 255);
		g = unsigned int(v * 255);
		b = unsigned int(p * 255);
		break;
	case 2:
		r = unsigned int(p * 255);
		g = unsigned int(v * 255);
		b = unsigned int(t * 255);
		break;
	case 3:
		r = unsigned int(p * 255);
		g = unsigned int(q * 255);
		b = unsigned int(v * 255);
		break;
	case 4:
		r = unsigned int(t * 255);
		g = unsigned int(p * 255);
		b = unsigned int(v * 255);
		break;
	default:		// case 5:
		r = unsigned int(v * 255);
		g = unsigned int(p * 255);
		b = unsigned int(q * 255);
		break;
	}
}

inline void getColorData(const unsigned int& pixel, unsigned int& r, unsigned int& g, unsigned int& b, unsigned int& a) restrict(amp, cpu)
{
	r = pixel & PIXEL_MASK;
	g = (pixel >> 8) & PIXEL_MASK;
	b = (pixel >> 16) & PIXEL_MASK;
	a = (pixel >> 24) & PIXEL_MASK;
}

inline unsigned int toPixel(unsigned int& r, unsigned int& g, unsigned int& b, unsigned int& a) restrict(amp, cpu)
{
	return r | (g << 8) | (b << 16) | (a << 24);
}

inline unsigned int colorDistance(unsigned int r1, unsigned int g1, unsigned int b1, unsigned int r2, unsigned int g2, unsigned int b2) restrict(amp, cpu)
{
	unsigned int dr = DIFF(r1, r2);
	unsigned int dg = DIFF(g1, g2);
	unsigned int db = DIFF(b1, b2);

	unsigned int sum = dr*dr + dg*dg + db*db;
	return unsigned int(Concurrency::fast_math::sqrt(float(sum)));
}

inline unsigned int edgeIntensity(unsigned int cx, unsigned int cy) restrict(amp, cpu)
{
	return unsigned int(Concurrency::fast_math::sqrt(float(cx*cx + cy*cy)));
}

inline void sort9(unsigned int* d) restrict(amp, cpu)
{
	// https://en.wikipedia.org/wiki/Sorting_network
	// http://stackoverflow.com/questions/2786899/fastest-sort-of-fixed-length-6-int-array
    #define SWAP(x,y) if (d[y] < d[x]) { unsigned int tmp = d[x]; d[x] = d[y]; d[y] = tmp; }
	SWAP(0, 1);
	SWAP(3, 4);
	SWAP(6, 7);
	SWAP(1, 2);
	SWAP(4, 5);
	SWAP(7, 8);
	SWAP(0, 1);
	SWAP(3, 4);
	SWAP(6, 7);
	SWAP(0, 3);
	SWAP(3, 6);
	SWAP(0, 3);
	SWAP(1, 4);
	SWAP(4, 7);
	SWAP(1, 4);
	SWAP(2, 5);
	SWAP(5, 8);
	SWAP(2, 5);
	SWAP(1, 3);
	SWAP(5, 7);
	SWAP(2, 6);
	SWAP(4, 6);
	SWAP(2, 4);
	SWAP(2, 3);
	SWAP(5, 6);
	#undef SWAP
}

	
