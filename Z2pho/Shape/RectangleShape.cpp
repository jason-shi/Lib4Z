#include "StdAfx.h"

CRectangleShape::CRectangleShape(int x1, int y1, int x2, int y2)
{
	//绘制和填充都不包含右边界和下边界
	this->x1 = min(x1, x2);
	this->x2 = max(x1, x2);
	this->y1 = min(y1, y2);
	this->y2 = max(y1, y2);
}

CRectangleShape::CRectangleShape(const CPoint& pt1, const CPoint& pt2)
					: x1(pt1.x), y1(pt1.y), x2(pt2.x), y2(pt2.y)
{
}

CRectangleShape::~CRectangleShape()
{
}

void CRectangleShape::_Draw(CCanvas *pCanvas)
{
	if (x1 == x2 || y1 == y2)
		return;

	//绘制两条水平线
	m_pImage->FillLine(x1, x2, y1, m_color);
	m_pImage->FillLine(x1, x2, y2 - 1, m_color);
	//绘制两条垂直线
	CLineShape tLine1(x1, y1, x1, y2);
	tLine1.SetColor(m_color);
	tLine1.Draw(pCanvas);

	CLineShape tLine2(x2 - 1, y1, x2 - 1, y2);
	tLine2.SetColor(m_color);
	tLine2.Draw(pCanvas);

	g_pZ2coo->Redraw(pCanvas);
}

void CRectangleShape::_Fill(CCanvas *pCanvas)
{
	for (int i = y1; i != y2; i++)
	{
		//绘制扫描线
		FillLine(x1, x2, i);
	}

	g_pZ2coo->Redraw(pCanvas);
}
