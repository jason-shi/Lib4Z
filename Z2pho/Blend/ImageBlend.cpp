#include "StdAfx.h"
#include "ImageBlend.h"

#define FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos) \
	for (int y = rect.top; y < rect.bottom; y++) \
	{ \
		COLORREF *p = (COLORREF *)pbg->GetBits(rect.left, y); \
		const COLORREF *q = (COLORREF *)pfg->GetBits(src_pos.x, src_pos.y++); \
		for (int x = rect.left; x < rect.right; x++) \
		{

#define FOREACH_PIXEL_END \
			p++; q++; \
		} \
	}

void CImageBlend::Normal(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Normal(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Behind(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Behind(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Dissolve(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Dissolve(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Opacity(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Opacity(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Darken(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Darken(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Lighten(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Lighten(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Multiply(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Multiply(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::LinearBurn(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::LinearBurn(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::LinearDodge(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::LinearDodge(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Screen(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Screen(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Difference(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Difference(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Exclusion(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Exclusion(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Average(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Average(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Negation(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Negation(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Phoenix(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Phoenix(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Interpolation(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Interpolation(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::ColorBurn(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::ColorBurn(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::ColorDodge(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::ColorDodge(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::ColorDark(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::ColorDark(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Overlay(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Overlay(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::SoftLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::SoftLight(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::HardLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::HardLight(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::VividLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::VividLight(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::LinearLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::LinearLight(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::PinLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::PinLight(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::HardMix(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::HardMix(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Reflect(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Reflect(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Glow(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Glow(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Freeze(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Freeze(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Heat(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Heat(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Hueration(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Hueration(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Saturation(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Saturation(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Coloration(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Coloration(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::Brightness(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::Brightness(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::BitXor(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::BitXor(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::BitAnd(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::BitAnd(*p, *q);
	FOREACH_PIXEL_END
}

void CImageBlend::BitOr(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos)
{
	FOREACH_PIXEL_BEGIN(pbg, pfg, rect, src_pos)
		*p = CColorBlend::BitOr(*p, *q);
	FOREACH_PIXEL_END
}
