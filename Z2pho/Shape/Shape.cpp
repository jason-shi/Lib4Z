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

	_Draw(pCanvas); //����ʵ�ֻ���
}

void CShape::Fill(CCanvas *pCanvas)
{
	m_pImage = pCanvas->GetImage();
	//Ϊ�����ָ��ͼ��
	if (m_pFill)
	{
		m_pFill->SetImage(m_pImage);
		m_pFill->InitFill();
	}

	_Fill(pCanvas); //����ʵ�����
}

void CShape::FillLine(int x1, int x2, int y)
{
	if (m_pFill)
	{
		//�Ƿ���ͼ���ڲ�
		if (y >= 0 && y < m_pImage->Height())
			m_pFill->FillLine(x1, x2, y);
	}
}
