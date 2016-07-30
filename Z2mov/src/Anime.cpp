#include "StdAfx.h"

CAnime::CAnime()
{
	m_bRemove = false;
	m_bActive = false;
	m_bPause = false;
	m_pNext = NULL;
}

CAnime::CAnime(int nFrame, int nPeriod, FN_AnimeEnd AnimeEnd, int *pParam)
{
	m_nFrame = nFrame;
	m_nPeriod = nPeriod;
	OnAnimeEnd = AnimeEnd;
	m_pParam = pParam;

	m_bRemove = false;
	m_bActive = false;
	m_bPause = false;
	m_pNext = NULL;
}

CAnime::~CAnime()
{
}

bool CAnime::AllocImage(int nWidth, int nHeight)
{
	SAFE_DELETE(m_pImage);

	m_pImage = new CImage(nWidth, nHeight);
	return m_pImage != NULL;
}

void CAnime::Play()
{
	if (m_bActive)
		return;

	//添加到队列
	g_pZ2mov->AddAnime(this);
	if (m_bRemove)
		g_pZ2coo->AddObject(m_pAttach);

	m_nStep = 0;
	m_nElapse = 0;
	m_bActive = true;
	m_bPause = false;

	SetUp();
}

void CAnime::Stop()
{
	if (!m_bActive)
		return;
	m_bActive = false;
	m_bPause = false;

	//从队列里删除
	g_pZ2mov->DelAnime(this);
	if (m_bRemove)
		g_pZ2coo->DelObject(m_pAttach);

	m_pAttach = NULL;
}

void CAnime::Pause()
{
	if (!m_bActive || m_bPause)
		return;
	m_bPause = true;
}

void CAnime::Resume()
{
	if (!m_bActive || !m_bPause)
		return;
	m_bPause = false;
}

void CAnime::SetUp()
{
}

void CAnime::Update(int nDelta)
{
	m_nElapse += nDelta;
	if (m_nElapse > m_nPeriod)
	{
		m_nElapse -= m_nPeriod;
		if (++m_nStep != m_nFrame)
			Update(nDelta);
		else
		{
			//动画正常结束
			m_bActive = false;

			//从队列里删除
			g_pZ2mov->DelAnime(this);
			if (m_bRemove)
				g_pZ2coo->DelObject(m_pAttach);

			g_pZ2coo->SendMessage(WM_ANIME_END, (WPARAM)this);
		}
	}
}
