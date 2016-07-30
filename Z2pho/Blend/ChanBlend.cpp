#include "StdAfx.h"
#include "ChanBlend.h"

double *CChanBlend::_TblCosine = NULL;

byte CChanBlend::Darken(byte bg, byte fg)
{
	return min(bg, fg);
}

byte CChanBlend::Lighten(byte bg, byte fg)
{
	return max(bg, fg);
}

byte CChanBlend::Multiply(byte bg, byte fg)
{
	return (bg * fg) >> 8;
}

byte CChanBlend::LinearBurn(byte bg, byte fg)
{
	return max(bg + fg - 255, 0);
}

byte CChanBlend::LinearDodge(byte bg, byte fg)
{
	return min(bg + fg, 255);
}

byte CChanBlend::Screen(byte bg, byte fg)
{
	return 255 - ((255 - bg) * (255 - fg) >> 8);
}

byte CChanBlend::Difference(byte bg, byte fg)
{
	return abs(bg - fg);
}

byte CChanBlend::Exclusion(byte bg, byte fg)
{
	return bg + fg - (bg * fg >> 7);
}

byte CChanBlend::Average(byte bg, byte fg)
{
	return (bg + fg) >> 1;
}

byte CChanBlend::Negation(byte bg, byte fg)
{
	return 255 - abs(255 - bg - fg);
}

byte CChanBlend::Phoenix(byte bg, byte fg)
{
	return min(bg, fg) - max(bg, fg) + 255;
}

byte CChanBlend::Interpolation(byte bg, byte fg)
{
	if (_TblCosine == NULL)
	{
		_TblCosine = new double[256];
		for (int i = 0; i < 255; i++)
			_TblCosine[i] = cos(i * M_PI / 255.0);
	}
	return min(255, 128 - 64 * _TblCosine[bg] - 64 * _TblCosine[fg]);
}

byte CChanBlend::ColorBurn(byte bg, byte fg)
{
	return fg == 0 ? 0 : max(0, 255 - ((255 - bg) << 8) / fg);
}

byte CChanBlend::ColorDodge(byte bg, byte fg)
{
	return fg == 255 ? 255 : min(255, (bg << 8) / (255 - fg));
}

byte CChanBlend::ColorDark(byte bg, byte fg)
{
	return min(255, (bg << 8) / (fg + 255));
}

byte CChanBlend::Overlay(byte bg, byte fg)
{
	return bg <= 128 ? bg * fg >> 7 : 255 - ((255 - fg) * (255 - bg) >> 7);
}

byte CChanBlend::SoftLight(byte bg, byte fg)
{
	return ((255 - bg) * bg * fg + bg * (65535 - (255 - bg) * ( 255 - fg))) >> 16;
}

byte CChanBlend::HardLight(byte bg, byte fg)
{
	return fg > 127 ? (65535 - (255 - bg) * (255 - (fg - 127) * 2)) >> 8 : bg * fg >> 7;
}

byte CChanBlend::VividLight(byte bg, byte fg)
{
	if (bg == 0 || bg == 255)
		return bg;

	return bg < 128 ? max(0, 255 - ((255 - fg) << 7) * bg) : min(255, (fg << 8) / (255 - (bg - 128) * 2));
}

byte CChanBlend::LinearLight(byte bg, byte fg)
{
	return bg < 128 ? max(0, (bg << 1) - fg) : min(255, ((bg - 128) << 1) + fg);
}

byte CChanBlend::PinLight(byte bg, byte fg)
{
	return fg < 128 ? min(fg << 1, bg) : max((fg - 128) << 1, bg);
}

byte CChanBlend::HardMix(byte bg, byte fg)
{
	return bg + fg < 255 ? 0 : 255;
}

byte CChanBlend::Reflect(byte bg, byte fg)
{
	return fg == 255 ? fg : min(255, bg * bg / (255 - fg));
}

byte CChanBlend::Glow(byte bg, byte fg)
{
	return bg == 255 ? bg : min(255, fg * fg / (255 - bg));
}

byte CChanBlend::Freeze(byte bg, byte fg)
{
	return fg == 0 ? 0 : max(0, 255 - (255 - bg) * (255 - bg) / fg);
}

byte CChanBlend::Heat(byte bg, byte fg)
{
	return bg == 0 ? 0 : max(0, 255 - (255 - fg) * (255 - fg) / bg);
}
