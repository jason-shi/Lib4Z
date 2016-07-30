#include "StdAfx.h"
#include "MandelCDS.h"

CMandelCDS::CMandelCDS(int N) : N(N)
{
}

CMandelCDS::~CMandelCDS()
{
}

void CMandelCDS::Draw(CCanvas *pCanvas)
{
	CImage *pImage = pCanvas->GetImage();
	CComplex delta((m_end.a - m_start.a) / pImage->Width(), (m_end.b - m_start.b) / pImage->Height());

	CComplex spot;
	for (int i = 0; i < pImage->Width(); i++)
	{
		spot.a = m_start.a + i * delta.a;
		for (int j = 0; j < pImage->Height(); j++)
		{
			spot.b = m_start.b + j * delta.b;

			int nIter = 0;
			CComplex c1(0, 0);
			while (nIter < m_nMaxIter && c1.GetModule() < m_fEscDist)
			{
				nIter++;
				c1 = cpower(c1, N) + spot;
			}
			pImage->SetPixel(i, j, GetColor(c1, nIter));
		}
	}
}

COLORREF CMandelCDS::GetColor(CComplex ret, int nIter)
{
	byte b = GetBValue(m_color);
	byte g = GetGValue(m_color);
	byte r = GetRValue(m_color);

	int newb = 256 - abs((int)abs(b - (2 * r - 256) * (log(abs(ret.a))) * m_fFactor) % 512 - 256);
	int newg = 256 - abs((int)abs(g - (2 * b - 256) * (log(abs((double)nIter))) * m_fFactor) % 512 - 256);
	int newr = 256 - abs((int)abs(r - (2 * g - 256) * (log(abs(ret.b))) * m_fFactor) % 512 - 256);

	return BGRA(max(0, min(255, newb)), max(0, min(255, newg)), max(0, min(255, newr)), 255);
}
