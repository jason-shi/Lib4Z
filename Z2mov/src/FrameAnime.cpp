#include "StdAfx.h"

CFrameAnime::CFrameAnime() : CAnime()
{
	m_pImage = NULL;
}

CFrameAnime::CFrameAnime(CImage *pImage, CRect xSrcRect, int nFrame, int nPeriod, FN_AnimeEnd AnimeEnd, int *pParam)
				: CAnime(nFrame, nPeriod, AnimeEnd, pParam)
{
	m_pImage = pImage;
	m_xSrcRect = xSrcRect;
}

CFrameAnime::~CFrameAnime()
{
}

void CFrameAnime::SetUp()
{
	m_xCurPos = m_xSrcRect.TopLeft();
	m_pAttach->SetSrcPos(m_xCurPos);

	m_pAttach->Show();
}

void CFrameAnime::Update(int nDelta)
{
	const CSize& xSize = m_pAttach->GetSize();

	//从左到右，从上到下
	m_xCurPos.x += xSize.cx;
	if (m_xCurPos.x + xSize.cx > m_xSrcRect.right)
	{
		m_xCurPos.x = m_xSrcRect.left;
		m_xCurPos.y += xSize.cy;
	}
	m_pAttach->SetSrcPos(m_xCurPos);
}
