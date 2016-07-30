#include "StdAfx.h"
#include "SketchFilter.h"

CSketchFilter::CSketchFilter(int nOutline, COLORREF color)
{
	m_nOutline = nOutline;
	m_color = color;
}

CSketchFilter::~CSketchFilter()
{
}

void CSketchFilter::Filter(CImage *pImage, const CRect& rect)
{
	//边缘检测卷积核
	int kernels[5][5] = {
		{ -1, -1, -1, -1, -1, },
		{ -1, -1, -1, -1, -1, },
		{ -1, -1, 26, -1, -1, },
		{ -1, -1, -1, -1, -1, },
		{ -1, -1, -1, -1, -1, },
	};

	CImage *pTemp = new CImage(pImage->Width(), pImage->Height());
	for (int y = rect.top; y < rect.bottom; y++)
	{
		for (int x = rect.left; x < rect.right; x++)
		{
			int blurB = 0, blurG = 0, blurR = 0, blurA = 0; //模糊色彩分量
			int outlB = 0, outlG = 0, outlR = 0, outlA = 0; //轮廓色彩分量
			for (int dy = -2; dy <= 2; dy++)
			{
				for (int dx = -2; dx <= 2; dx++)
				{
					int _x = min(max(0, x + dx), pImage->Width() - 1);
					int _y = min(max(0, y + dy), pImage->Height() - 1);
					byte *q = (byte *)pImage->GetBits(_x, _y);

					int factor = kernels[2 + dy][2 + dx];
					blurB += *q; outlB += *q++ * factor;
					blurG += *q; outlG += *q++ * factor;
					blurR += *q; outlR += *q++ * factor;
					blurA += *q; outlA += *q++ * factor;
				}
			}
			blurB /= 25; blurG /= 25; blurR /= 25; blurA /= 25;

			//高斯模糊后与原图滤色合成
			COLORREF bot = pImage->GetPixel(x, y);
			bot = CColorBlend::Screen(bot, BGRA(blurB, blurG, blurR, blurA));

			//边缘增强并去色处理
			outlB = min(max(0, outlB), 255);
			outlG = min(max(0, outlG), 255);
			outlR = min(max(0, outlR), 255);
			outlA = min(max(0, outlA), 255);
			float gray = 0.299f * outlR + 0.587f * outlG + 0.114f * outlB;
			COLORREF top;
			if (gray > m_nOutline * 255 / 100)
			{
				top = BGRA(255, 255, 255, outlA);
				bot = top;
			}
			else
			{
				//对着色区进行变暗合成
				top = BGRA(GetBValue(m_color), GetGValue(m_color), GetRValue(m_color), outlA);
				bot = CColorBlend::Darken(bot, top);
			}
			pTemp->SetPixel(x, y, bot);
		}
	}
	pImage->Copy(pTemp, rect);
	delete pTemp;
}
