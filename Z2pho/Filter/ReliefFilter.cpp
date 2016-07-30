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
//        cos(��+45��) cos(��+90��) cos(��+135��) //
// �����   cos(��)         0       cos(��+180��) //
//        cos(��-45��) cos(��-90��) cos(��-135��) //
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
