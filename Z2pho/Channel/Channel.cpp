#include "StdAfx.h"
#include "Channel.h"

#define FOREACH_PIXEL_BEGIN \
	for (int y = 0; y < pImage->Height(); y++) \
	{ \
		COLORREF *p = (COLORREF *)GetBits(0, y); \
		const COLORREF *q = (COLORREF *)pImage->GetBits(0, y); \
		for (int x = 0; x < pImage->Width(); x++) \
		{

#define FOREACH_PIXEL_END \
			p++; q++; \
		} \
	}

void CChannel::GetChannel(const CImage *pImage)
{
	//若图象大小不同，则重建图象
	Create(pImage->Width(), pImage->Height());

	//根据通道类型，获取通道值
	switch (m_nChlType)
	{
	case CHL_BLUE:
		BGRA_BChan(pImage);
		break;
	case CHL_GREEN:
		BGRA_GChan(pImage);
		break;
	case CHL_RED:
		BGRA_RChan(pImage);
		break;
	case CHL_ALPHA:
		BGRA_AChan(pImage);
		break;
	case CHL_BLUE | CHL_GREEN:
		BGRA_BGChan(pImage);
		break;
	case CHL_BLUE | CHL_RED:
		BGRA_BRChan(pImage);
		break;
	case CHL_GREEN | CHL_RED:
		BGRA_GRChan(pImage);
		break;
	case CHL_BLUE | CHL_GREEN | CHL_RED:
		BGRA_BGRChan(pImage);
		break;
	case CHL_HUE:
		HSB_HChan(pImage);
		break;
	case CHL_SATUR:
		HSB_SChan(pImage);
		break;
	case CHL_BRIGHT:
		HSB_BChan(pImage);
		break;
	case CHL_HUE | CHL_SATUR:
		HSB_HSChan(pImage);
		break;
	case CHL_HUE | CHL_BRIGHT:
		HSB_HBChan(pImage);
		break;
	case CHL_SATUR | CHL_BRIGHT:
		HSB_SBChan(pImage);
		break;
	case CHL_HUE | CHL_SATUR | CHL_BRIGHT:
		HSB_HSBChan(pImage);
		break;
	}
}

void CChannel::BGRA_BChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		byte b = GetBValue(*q);
		*p = BGRA(b, b, b, 255);
	FOREACH_PIXEL_END
}

void CChannel::BGRA_GChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		byte g = GetGValue(*q);
		*p = BGRA(g, g, g, 255);
	FOREACH_PIXEL_END
}

void CChannel::BGRA_RChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		byte r = GetRValue(*q);
		*p = BGRA(r, r, r, 255);
	FOREACH_PIXEL_END
}

void CChannel::BGRA_AChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		byte a = GetAValue(*q);
		*p = BGRA(a, a, a, 255);
	FOREACH_PIXEL_END
}

void CChannel::BGRA_BGChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		byte b = GetBValue(*q);
		byte g = GetGValue(*q);
		*p = BGRA(b, g, 0, 255);
	FOREACH_PIXEL_END
}

void CChannel::BGRA_BRChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		byte b = GetBValue(*q);
		byte r = GetRValue(*q);
		*p = BGRA(b, 0, r, 255);
	FOREACH_PIXEL_END
}

void CChannel::BGRA_GRChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		byte g = GetGValue(*q);
		byte r = GetRValue(*q);
		*p = BGRA(0, g, r, 255);
	FOREACH_PIXEL_END
}

void CChannel::BGRA_BGRChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		byte b = GetBValue(*q);
		byte g = GetGValue(*q);
		byte r = GetRValue(*q);
		*p = BGRA(b, g, r, 255);
	FOREACH_PIXEL_END
}

void CChannel::HSB_HChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		CColorSpace::HSB hsb = CColorSpace::HSB::FromBGRA(*q);
		byte H = hsb.H / 360.0 * 255.0; //以色相作为灰度
		*p = BGRA(H, H, H, 255);
	FOREACH_PIXEL_END
}

void CChannel::HSB_SChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		CColorSpace::HSB hsb = CColorSpace::HSB::FromBGRA(*q);
		byte S = hsb.S * 255.0; //以饱和度作为灰度
		*p = BGRA(S, S, S, 255);
	FOREACH_PIXEL_END
}

void CChannel::HSB_BChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		CColorSpace::HSB hsb = CColorSpace::HSB::FromBGRA(*q);
		byte B = hsb.B * 255.0; //以明度作为灰度
		*p = BGRA(B, B, B, 255);
	FOREACH_PIXEL_END
}

void CChannel::HSB_HSChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		CColorSpace::HSB hsb = CColorSpace::HSB::FromBGRA(*q);
		hsb.B = 1.0f; //忽略明度
		*p = hsb.ToBGRA();
	FOREACH_PIXEL_END
}

void CChannel::HSB_HBChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		CColorSpace::HSB hsb = CColorSpace::HSB::FromBGRA(*q);
		hsb.S = 0; //忽略饱和度
		*p = hsb.ToBGRA();
	FOREACH_PIXEL_END
}

void CChannel::HSB_SBChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		CColorSpace::HSB hsb = CColorSpace::HSB::FromBGRA(*q);
		hsb.H = 0; //忽略色相
		*p = hsb.ToBGRA();
	FOREACH_PIXEL_END
}

void CChannel::HSB_HSBChan(const CImage *pImage)
{
	FOREACH_PIXEL_BEGIN
		CColorSpace::HSB hsb = CColorSpace::HSB::FromBGRA(*q);
		*p = hsb.ToBGRA();
	FOREACH_PIXEL_END
}
