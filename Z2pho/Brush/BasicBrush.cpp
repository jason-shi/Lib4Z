#include "StdAfx.h"
#include "BasicBrush.h"

CBasicBrush::CBasicBrush(int nDiameter)
{
	m_nDiameter = nDiameter;
}

void CBasicBrush::InitBrush()
{
	int nRadius = m_nDiameter / 2;
	CCanvas *pCanvas = new CCanvas(NULL, CPoint(0, 0), CSize(nRadius, nRadius), 0);
	
	CCircleShape tCircleShape(nRadius, nRadius, nRadius);
	tCircleShape.Fill(pCanvas);
}

void CBasicBrush::Draw(CCanvas *pCanvas)
{
}
