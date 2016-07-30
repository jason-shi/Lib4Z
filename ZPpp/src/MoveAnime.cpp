#include "StdAfx.h"

CMoveAnime::CMoveAnime() : CAnime()
{
}

CMoveAnime::CMoveAnime(CPoint xSrcPoint, CPoint xDestPoint, int nFrame, int nPeriod, FN_AnimeEnd AnimeEnd, int *pParam)
				: CAnime(nFrame, nPeriod, AnimeEnd, pParam)
{
	m_xSrcPoint = xSrcPoint;
	m_xDestPoint = xDestPoint;
}

CMoveAnime::~CMoveAnime()
{
}

void CMoveAnime::SetSrcPoint(int x, int y)
{
	if (IsActive())
		return;
	m_xSrcPoint.x = x;
	m_xSrcPoint.y = y;
}

void CMoveAnime::SetDestPoint(int x, int y)
{
	if (IsActive())
		return;
	m_xDestPoint.x = x;
	m_xDestPoint.y = y;
}

void CMoveAnime::SetUp()
{
	dx = (float)(m_xDestPoint.x - m_xSrcPoint.x) / m_nFrame;
	dy = (float)(m_xDestPoint.y - m_xSrcPoint.y) / m_nFrame;

	m_pAttach->SetDrawPos(m_xSrcPoint);

	m_pAttach->Show();
}

void CMoveAnime::Update(int nDelta)
{
	CPoint xDrawPos;
	xDrawPos.x = m_xSrcPoint.x + m_nStep * dx;
	xDrawPos.y = m_xSrcPoint.y + m_nStep * dy;

	m_pAttach->SetDrawPos(xDrawPos);
}
