#include "StdAfx.h"
#include "Shape.h"

CShape::CShape()
{
	m_pFill = NULL;
}

CShape::~CShape()
{
}

void CShape::Draw(CCanvas *pCanvas)
{
	m_pImage = pCanvas->GetImage();

	_Draw(pCanvas); //子类实现绘制
}

void CShape::Fill(CCanvas *pCanvas)
{
	m_pImage = pCanvas->GetImage();
	//为填充类指定图象
	if (m_pFill)
	{
		m_pFill->SetImage(m_pImage);
		m_pFill->InitFill();
	}

	_Fill(pCanvas); //子类实现填充
}

void CShape::FillLine(int x1, int x2, int y)
{
	if (m_pFill)
	{
		//是否在图象内部
		if (y >= 0 && y < m_pImage->Height())
			m_pFill->FillLine(x1, x2, y);
	}
}
