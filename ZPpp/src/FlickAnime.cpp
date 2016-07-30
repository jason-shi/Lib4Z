#include "StdAfx.h"

CFlickAnime::CFlickAnime() : CAnime()
{
}

CFlickAnime::CFlickAnime(int nFrame, int nShowPrd, int nHidePrd, bool bPriHide) : CAnime(nFrame, 0)
{
	m_nShowPrd = nShowPrd;
	m_nHidePrd = nHidePrd;
	m_bPriHide = bPriHide;
}

CFlickAnime::~CFlickAnime()
{
}

void CFlickAnime::SetUp()
{
	m_nPeriod = m_bPriHide ? m_nHidePrd : m_nShowPrd;
	m_pAttach->Show(!m_bPriHide);
}

void CFlickAnime::Update(int nDelta)
{
	bool bShow = m_pAttach->IsShow();

	//ÏÔÊ¾/Òþ²Ø·´Ïà
	m_nPeriod = bShow ? m_nHidePrd : m_nShowPrd;
	m_pAttach->Show(!bShow);
}
