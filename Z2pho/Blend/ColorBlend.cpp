#include "StdAfx.h"
#include "ColorBlend.h"

COLORREF CColorBlend::Normal(COLORREF bg, COLORREF fg)
{
	return fg;
}

COLORREF CColorBlend::Behind(COLORREF bg, COLORREF fg)
{
	return bg;
}

COLORREF CColorBlend::Dissolve(COLORREF bg, COLORREF fg)
{
	return (rand() >> 8) < fg ? fg : bg;
}

COLORREF CColorBlend::Opacity(COLORREF bg, COLORREF fg)
{
	COLORREF ret;
	byte *r = (byte *)&ret;
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte alpha = GetAValue(fg);

	alpha_composite(*r++, *p++, alpha, *q++);
	alpha_composite(*r++, *p++, alpha, *q++);
	alpha_composite(*r++, *p++, alpha, *q++);
	alpha_composite(*r++, *p++, alpha, *q++);
	return ret;
}

COLORREF CColorBlend::Darken(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Darken(*p++, *q++);
	byte g = CChanBlend::Darken(*p++, *q++);
	byte r = CChanBlend::Darken(*p++, *q++);
	byte a = CChanBlend::Darken(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Lighten(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Lighten(*p++, *q++);
	byte g = CChanBlend::Lighten(*p++, *q++);
	byte r = CChanBlend::Lighten(*p++, *q++);
	byte a = CChanBlend::Lighten(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Multiply(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Multiply(*p++, *q++);
	byte g = CChanBlend::Multiply(*p++, *q++);
	byte r = CChanBlend::Multiply(*p++, *q++);
	byte a = CChanBlend::Multiply(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::LinearBurn(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::LinearBurn(*p++, *q++);
	byte g = CChanBlend::LinearBurn(*p++, *q++);
	byte r = CChanBlend::LinearBurn(*p++, *q++);
	byte a = CChanBlend::LinearBurn(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::LinearDodge(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::LinearDodge(*p++, *q++);
	byte g = CChanBlend::LinearDodge(*p++, *q++);
	byte r = CChanBlend::LinearDodge(*p++, *q++);
	byte a = CChanBlend::LinearDodge(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Screen(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Screen(*p++, *q++);
	byte g = CChanBlend::Screen(*p++, *q++);
	byte r = CChanBlend::Screen(*p++, *q++);
	byte a = CChanBlend::Screen(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Difference(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Difference(*p++, *q++);
	byte g = CChanBlend::Difference(*p++, *q++);
	byte r = CChanBlend::Difference(*p++, *q++);
	byte a = CChanBlend::Difference(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Exclusion(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Exclusion(*p++, *q++);
	byte g = CChanBlend::Exclusion(*p++, *q++);
	byte r = CChanBlend::Exclusion(*p++, *q++);
	byte a = CChanBlend::Exclusion(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Average(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Average(*p++, *q++);
	byte g = CChanBlend::Average(*p++, *q++);
	byte r = CChanBlend::Average(*p++, *q++);
	byte a = CChanBlend::Average(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Negation(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Negation(*p++, *q++);
	byte g = CChanBlend::Negation(*p++, *q++);
	byte r = CChanBlend::Negation(*p++, *q++);
	byte a = CChanBlend::Negation(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Phoenix(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Phoenix(*p++, *q++);
	byte g = CChanBlend::Phoenix(*p++, *q++);
	byte r = CChanBlend::Phoenix(*p++, *q++);
	byte a = CChanBlend::Phoenix(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Interpolation(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Interpolation(*p++, *q++);
	byte g = CChanBlend::Interpolation(*p++, *q++);
	byte r = CChanBlend::Interpolation(*p++, *q++);
	byte a = CChanBlend::Interpolation(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::ColorBurn(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::ColorBurn(*p++, *q++);
	byte g = CChanBlend::ColorBurn(*p++, *q++);
	byte r = CChanBlend::ColorBurn(*p++, *q++);
	byte a = CChanBlend::ColorBurn(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::ColorDodge(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::ColorDodge(*p++, *q++);
	byte g = CChanBlend::ColorDodge(*p++, *q++);
	byte r = CChanBlend::ColorDodge(*p++, *q++);
	byte a = CChanBlend::ColorDodge(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::ColorDark(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::ColorDark(*p++, *q++);
	byte g = CChanBlend::ColorDark(*p++, *q++);
	byte r = CChanBlend::ColorDark(*p++, *q++);
	byte a = CChanBlend::ColorDark(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Overlay(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Overlay(*p++, *q++);
	byte g = CChanBlend::Overlay(*p++, *q++);
	byte r = CChanBlend::Overlay(*p++, *q++);
	byte a = CChanBlend::Overlay(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::SoftLight(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::SoftLight(*p++, *q++);
	byte g = CChanBlend::SoftLight(*p++, *q++);
	byte r = CChanBlend::SoftLight(*p++, *q++);
	byte a = CChanBlend::SoftLight(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::HardLight(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::HardLight(*p++, *q++);
	byte g = CChanBlend::HardLight(*p++, *q++);
	byte r = CChanBlend::HardLight(*p++, *q++);
	byte a = CChanBlend::HardLight(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::VividLight(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::VividLight(*p++, *q++);
	byte g = CChanBlend::VividLight(*p++, *q++);
	byte r = CChanBlend::VividLight(*p++, *q++);
	byte a = CChanBlend::VividLight(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::LinearLight(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::LinearLight(*p++, *q++);
	byte g = CChanBlend::LinearLight(*p++, *q++);
	byte r = CChanBlend::LinearLight(*p++, *q++);
	byte a = CChanBlend::LinearLight(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::PinLight(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::PinLight(*p++, *q++);
	byte g = CChanBlend::PinLight(*p++, *q++);
	byte r = CChanBlend::PinLight(*p++, *q++);
	byte a = CChanBlend::PinLight(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::HardMix(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::HardMix(*p++, *q++);
	byte g = CChanBlend::HardMix(*p++, *q++);
	byte r = CChanBlend::HardMix(*p++, *q++);
	byte a = CChanBlend::HardMix(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Reflect(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Reflect(*p++, *q++);
	byte g = CChanBlend::Reflect(*p++, *q++);
	byte r = CChanBlend::Reflect(*p++, *q++);
	byte a = CChanBlend::Reflect(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Glow(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Glow(*p++, *q++);
	byte g = CChanBlend::Glow(*p++, *q++);
	byte r = CChanBlend::Glow(*p++, *q++);
	byte a = CChanBlend::Glow(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Freeze(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Freeze(*p++, *q++);
	byte g = CChanBlend::Freeze(*p++, *q++);
	byte r = CChanBlend::Freeze(*p++, *q++);
	byte a = CChanBlend::Freeze(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Heat(COLORREF bg, COLORREF fg)
{
	byte *p = (byte *)&bg;
	byte *q = (byte *)&fg;

	byte b = CChanBlend::Heat(*p++, *q++);
	byte g = CChanBlend::Heat(*p++, *q++);
	byte r = CChanBlend::Heat(*p++, *q++);
	byte a = CChanBlend::Heat(*p++, *q++);
	return BGRA(b, g, r, a);
}

COLORREF CColorBlend::Hueration(COLORREF bg, COLORREF fg)
{
	CColorSpace::HSB hsb1 = CColorSpace::HSB::FromBGRA(bg);
	CColorSpace::HSB hsb2 = CColorSpace::HSB::FromBGRA(fg);
	hsb1.H = hsb2.H;
	return hsb1.ToBGRA();
}

COLORREF CColorBlend::Saturation(COLORREF bg, COLORREF fg)
{
	CColorSpace::HSB hsb1 = CColorSpace::HSB::FromBGRA(bg);
	CColorSpace::HSB hsb2 = CColorSpace::HSB::FromBGRA(fg);
	hsb1.S = hsb2.S;
	return hsb1.ToBGRA();
}

COLORREF CColorBlend::Coloration(COLORREF bg, COLORREF fg)
{
	CColorSpace::HSB hsb1 = CColorSpace::HSB::FromBGRA(bg);
	CColorSpace::HSB hsb2 = CColorSpace::HSB::FromBGRA(fg);
	hsb1.H = hsb2.H;
	hsb1.S = hsb2.S;
	return hsb1.ToBGRA();
}

COLORREF CColorBlend::Brightness(COLORREF bg, COLORREF fg)
{
	CColorSpace::HSB hsb1 = CColorSpace::HSB::FromBGRA(bg);
	CColorSpace::HSB hsb2 = CColorSpace::HSB::FromBGRA(fg);
	hsb1.B = hsb2.B;
	return hsb1.ToBGRA();
}

COLORREF CColorBlend::BitXor(COLORREF bg, COLORREF fg)
{
	return bg ^ fg;
}

COLORREF CColorBlend::BitAnd(COLORREF bg, COLORREF fg)
{
	return bg & fg;
}

COLORREF CColorBlend::BitOr(COLORREF bg, COLORREF fg)
{
	return bg | fg;
}
