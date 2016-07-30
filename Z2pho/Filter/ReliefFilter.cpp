#include "StdAfx.h"
#include "ReliefFilter.h"

CReliefFilter::CReliefFilter(float fAngle)
{
	m_fAngle = fAngle;
}

CReliefFilter::~CReliefFilter()
{
}

////////////////////////////////////////////////////
//        cos(¦È+45¡ã) cos(¦È+90¡ã) cos(¦È+135¡ã) //
// ¾í»ýºË   cos(¦È)         0       cos(¦È+180¡ã) //
//        cos(¦È-45¡ã) cos(¦È-90¡ã) cos(¦È-135¡ã) //
////////////////////////////////////////////////////
void CReliefFilter::Filter(CImage *pImage, const CRect& rect)
{
	float r = m_fAngle * M_PI / 180.0;
	float dr = (float)M_PI / 4.0;

	float kernels[9] = {
		cos(r + dr), cos(r + 2.0 * dr), cos(r + 3.0 * dr),
		cos(r), 0, cos(r + 4.0 * dr),
		cos(r - dr), cos(r - 2.0 * dr), cos(r - 3.0 * dr)
	};

	pImage->Convolve(rect, kernels);
}
