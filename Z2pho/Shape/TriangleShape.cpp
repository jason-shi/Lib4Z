#include "StdAfx.h"

CTriangleShape::CTriangleShape(int nCount, LPPOINT lpPoints)
					: m_nCount(nCount), lpPoints(lpPoints)
{
}

CTriangleShape::~CTriangleShape()
{
}

void CTriangleShape::_Draw(CCanvas *pCanvas)
{
}

void CTriangleShape::_Fill(CCanvas *pCanvas)
{
}
