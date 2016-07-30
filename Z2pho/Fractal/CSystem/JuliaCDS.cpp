#include "StdAfx.h"
#include "JuliaCDS.h"

CJuliaCDS::CJuliaCDS(int N, const CComplex& C0) : CMandelCDS(N), C0(C0)
{
}

CJuliaCDS::~CJuliaCDS()
{
}

void CJuliaCDS::Draw(CCanvas *pCanvas)
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
			CComplex c1(spot);
			while (nIter < m_nMaxIter && c1.GetModule() < m_fEscDist)
			{
				nIter++;
				c1 = cpower(c1, N) + C0;
			}
			pImage->SetPixel(i, j, GetColor(c1, nIter));
		}
	}
}
