#include "StdAfx.h"
#include "ColorSpace.h"

using namespace CColorSpace;

HSB HSB::FromBGRA(COLORREF color)
{
	byte *p = (byte *)&color;

	int min = p[0] < p[1] && p[0] < p[2] ? 0 : p[1] < p[0] && p[1] < p[2] ? 1 : 2;
	int max = p[2] > p[0] && p[2] > p[1] ? 2 : p[1] > p[0] && p[1] > p[2] ? 1 : 0;
	int mid = 3 - min - max;

	HSB hsb;
	hsb.B = p[max] / 255.0;
	hsb.S = 1 - (float)p[min] / p[max];
	hsb.H = (2 - max) * 120 + 60 * (p[mid] / hsb.S / p[max] + (1 - 1 / hsb.S)) * ((min - max + 3) % 3 == 1 ? 1 : -1);
	hsb.H = (int)(hsb.H + 360) % 360;
	return hsb;
}

COLORREF HSB::ToBGRA()
{
	float rgb[3];

	for (int offset = 240, i = 0; i < 3; i++, offset -= 120)
	{
		float x = abs((int)(H + offset) % 360 - 240);
		if (x <= 60)
			rgb[i] = 255;
		else if (60 < x && x < 120)
			rgb[i] = (1 - (x - 60) / 60) * 255;
		else
			rgb[i] = 0;
	}
	for (int i = 0; i < 3; i++)
		rgb[i] += (255 - rgb[i]) * (1 - S);
	for (int i = 0; i < 3; i++)
		rgb[i] *= B;

	return BGRA(rgb[2], rgb[1], rgb[0], 255);
}
