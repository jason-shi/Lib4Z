#include "StdAfx.h"

CRotateAnime::CRotateAnime() : CAnime()
{
	m_pImage = NULL;
}

CRotateAnime::CRotateAnime(CPoint xFixPoint, float fAngle, int nFrame, int nPeriod, FN_AnimeEnd AnimeEnd, int *pParam)
				: CAnime(nFrame, nPeriod, AnimeEnd, pParam)
{
	m_xFixPoint = xFixPoint;
	m_fAngle = fAngle;
	m_pImage = NULL;
}

CRotateAnime::~CRotateAnime()
{
}

void CRotateAnime::SetUp()
{
	const CSize& xSize = m_pAttach->GetSize();
	int nLength = sqrt((float)(xSize.cx * xSize.cx + xSize.cy * xSize.cy));

	delete m_pImage;
	m_pImage = new CImage(nLength, nLength);

	//¼ÆËãÐý×ª¾ØÕó


	m_pAttach->Show();
}

void CRotateAnime::Update(int nDelta)
{
	//CRect r;
	//GetRect(&r);

	//g_pZ2coo->Redraw(this, r);
}
