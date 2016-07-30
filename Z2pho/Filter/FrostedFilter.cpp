#include "StdAfx.h"
#include "FrostedFilter.h"

CFrostedFilter::CFrostedFilter(int nMinRadius, int nMaxRadius, int nSamples)
{
	m_nMinRadius = nMinRadius;
	m_nMaxRadius = nMaxRadius;
	m_nSamples = nSamples;
}

CFrostedFilter::~CFrostedFilter()
{
}

void CFrostedFilter::Filter(CImage *pImage, const CRect& rect)
{
	//环形域内的随机样本
	COLORREF *pSamples = new COLORREF[m_nSamples];

	CImage *pTemp = new CImage(pImage->Width(), pImage->Height());
	for (int y = rect.top; y < rect.bottom; y++)
	{
		for (int x = rect.left; x < rect.right; x++)
		{
			for (int s = 0; s < m_nSamples; s++)
			{
				double distance = RandDbl(m_nMinRadius, m_nMaxRadius); //随机离散距离
				double angle = RandDbl(0, 1) * M_PI * 2; //随机偏转角度

				int sampx = x + cos(angle) * distance; //水平采样点坐标
				int sampy = y + sin(angle) * distance; //垂直采样点坐标
				sampx = min(max(0, sampx), pImage->Width() - 1);
				sampy = min(max(0, sampy), pImage->Height() - 1);

				pSamples[s] = pImage->GetPixel(sampx, sampy);
			}
			COLORREF color = BlendColors(pSamples, m_nSamples);
			pTemp->SetPixel(x, y, color);
		}
	}
	pImage->Copy(pTemp, rect);
	delete pTemp;

	delete pSamples;
}

COLORREF CFrostedFilter::BlendColors(COLORREF colors[], int nLength)
{
	int bsum = 0, gsum = 0, rsum = 0, asum = 0;
	//进行Alpha混合模糊
	for (int i = 0; i < nLength; i++)
	{
		asum += GetAValue(colors[i]);
		bsum += GetAValue(colors[i]) * GetBValue(colors[i]);
		gsum += GetAValue(colors[i]) * GetGValue(colors[i]);
		rsum += GetAValue(colors[i]) * GetRValue(colors[i]);
	}
	//混合结果为透明
	if (asum == 0)
		return BGRA(0, 0, 0, 0);

	return BGRA(bsum / asum, gsum / asum, rsum / asum, asum / nLength);
}
