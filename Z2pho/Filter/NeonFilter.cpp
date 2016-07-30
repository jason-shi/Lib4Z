#include "StdAfx.h"
#include "NeonFilter.h"

#define SQUARE(x)		((x) * (x))
#define SUMSQR(x1, x2)	(SQUARE(x1) + SQUARE(x2))	

CNeonFilter::CNeonFilter(int nWidth, float fFactor)
{
	m_nWidth = nWidth;
	m_fFactor = fFactor;
}

CNeonFilter::~CNeonFilter()
{
}

void CNeonFilter::Filter(CImage *pImage, const CRect& rect)
{
	CImage *pTemp = new CImage(pImage->Width(), pImage->Height());
	for (int y = rect.top; y < rect.bottom; y++)
	{
		byte *p = (byte *)pTemp->GetBits(rect.left, y);
		const byte *q = (byte *)pImage->GetBits(rect.left, y);

		const byte *qV = (byte *)pImage->GetBits(rect.left, min(y + m_nWidth, pImage->Height() - 1));
		for (int x = rect.left; x < rect.right; x++)
		{
			//保证qH, qV在界内
			const byte *qH = (byte *)pImage->GetBits(min(x + m_nWidth, pImage->Width() - 1), y);

			//水平梯度，垂直梯度的平方和的开方 * 增强系数
			*p++ = min(m_fFactor * sqrt((double)SUMSQR(*q - *qH, *q - *qV)), 255); q++; qH++, qV++;
			*p++ = min(m_fFactor * sqrt((double)SUMSQR(*q - *qH, *q - *qV)), 255); q++; qH++, qV++;
			*p++ = min(m_fFactor * sqrt((double)SUMSQR(*q - *qH, *q - *qV)), 255); q++; qH++, qV++;
			*p++ = *q++; qH++; qV++;
		}
	}
	pImage->Copy(pTemp, rect);
	delete pTemp;
}
