#include "StdAfx.h"
#include "ImageBrush.h"

CImageBrush::CImageBrush(CImage *pImage)
{
	m_pImage = pImage;
}

void CImageBrush::InitBrush()
{
	m_pImage->LoadImage(L"Brush\\Basic Brushes");
}

void CImageBrush::Draw(CCanvas *pCanvas)
{
	CImage *pImage = pCanvas->GetImage();

	int BW = m_pImage->Width();
	int BH = m_pImage->Height();

	CSprite tSprite(m_pImage, CPoint(), CSize(BW, BH), 0);
	//以中心点沿路径绘制
	for (int i = 0; i < m_nCount; i += m_nDynamics)
	{
		tSprite.SetDrawPos(m_lpPoints[i].x - (BW >> 1), m_lpPoints[i].y - (BH >> 1));
		tSprite.Draw(pImage);
	}
	g_pZ2coo->Redraw(pCanvas);
}
