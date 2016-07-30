#include "StdAfx.h"

CZoomAnime::CZoomAnime() : CAnime()
{
	m_pImage = NULL;
}

CZoomAnime::CZoomAnime(float fRatioX, float fRatioY, int nFrame, int nPeriod, FN_AnimeEnd AnimeEnd, int *pParam)
				: CAnime(nFrame, nPeriod, AnimeEnd, pParam)
{
	m_fRatioX = fRatioX;
	m_fRatioY = fRatioY;
	m_pImage = NULL;
}

CZoomAnime::~CZoomAnime()
{
	delete m_pImage;
	m_pImage = NULL;
}

void CZoomAnime::SetUp()
{
	CRect rect;
	m_pAttach->GetRect(&rect);
	//保存图像
	m_pImage = new CImage(rect.Width() * m_fRatioX, rect.Height() * m_fRatioY);
	m_pImage->Copy(m_pAttach->GetImage(), rect);

	TmpRatioX = 1.0;
	TmpRatioY = 1.0;

	m_pAttach->Show();
}

void CZoomAnime::Update(int nDelta)
{
	//CRect r;
	//GetRect(&r);

	//m_xDrawPos.x = m_xSrcPoint.x + m_nStep * dx;
	//m_xDrawPos.y = m_xSrcPoint.y + m_nStep * dy;

	////更新缩放比率
	//TmpRatioX *= m_fRatioX;
	//TmpRatioY *= m_fRatioY;

	//m_pImage->Clear();
	//m_pImage->ZoomRect(m_pImage, m_xSrcRect, TmpRatioX, TmpRatioY);

	//g_pZ2coo->Redraw(this, r);
}
