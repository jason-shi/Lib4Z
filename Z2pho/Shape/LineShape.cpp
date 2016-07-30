#include "StdAfx.h"

CLineShape::CLineShape(int x1, int y1, int x2, int y2)
				: x1(x1), y1(y1), x2(x2), y2(y2)
{
}

CLineShape::CLineShape(const CPoint& pt1, const CPoint& pt2)
				: x1(pt1.x), y1(pt1.y), x2(pt2.x), y2(pt2.y)
{
}

CLineShape::~CLineShape()
{
}

void CLineShape::_Draw(CCanvas *pCanvas)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1; //x的增量方向，取1或-1
	int uy = ((dy > 0) << 1) - 1; //y的增量方向，取1或-1
	int x = x1, y = y1, eps = 0; //eps为累加误差

	dx = abs(dx);
	dy = abs(dy);
	if (dx > dy)
	{
		for (int x = x1; x != x2; x += ux)
		{
			m_pImage->SetPixel(x, y, m_color);
			eps += dy;
			if ((eps << 1) >= dx)
			{
				y += uy;
				eps -= dx;
			}
		}
	}
	else
	{
		for (int y = y1; y != y2; y += uy)
		{
			m_pImage->SetPixel(x, y, m_color);
			eps += dx;
			if ((eps << 1) >= dy)
			{
				x += ux;
				eps -= dy;
			}
		}
	}
	g_pZ2coo->Redraw(pCanvas);
}

void CLineShape::_Fill(CCanvas *pCanvas)
{
	_Draw(pCanvas);
}
