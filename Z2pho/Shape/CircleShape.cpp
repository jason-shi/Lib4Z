#include "StdAfx.h"

CCircleShape::CCircleShape(int xCenter, int yCenter, int Rc)
				: xCenter(xCenter), yCenter(yCenter), Rc(Rc)
{
}

CCircleShape::~CCircleShape()
{
}          

void CCircleShape::_Draw(CCanvas *pCanvas)
{
	int x = 0, y = Rc;
	int p = 1 - Rc;

	//plot first set of points
	_CirclePlot(xCenter, yCenter, x, y);
	while (x < y)
	{
		x++;
		if (p < 0)
			p += 2 * x + 1;
		else
		{
			y--;
			p += 2 * (x - y) + 1;
		}
		_CirclePlot(xCenter, yCenter, x, y);
	}
	g_pZ2coo->Redraw(pCanvas);
}

void CCircleShape::_Fill(CCanvas *pCanvas)
{
	int x = 0, y = Rc;
	int p = 1 - Rc;

	//fill first set of points
	_CircleFill(xCenter, yCenter, x, y);
	while (x < y)
	{
		x++;
		if (p < 0)
			p += 2 * x + 1;
		else
		{
			y--;
			p += 2 * (x - y) + 1;
		}
		_CircleFill(xCenter, yCenter, x, y);
	}
	g_pZ2coo->Redraw(pCanvas);
}

void CCircleShape::_CirclePlot(int xCenter, int yCenter, int x, int y)
{
	m_pImage->SetPixel(xCenter + x - 1, yCenter + y - 1, m_color);
	m_pImage->SetPixel(xCenter + x - 1, yCenter - y,     m_color);
	m_pImage->SetPixel(xCenter - x,     yCenter + y - 1, m_color);
	m_pImage->SetPixel(xCenter - x,     yCenter - y,     m_color);
	m_pImage->SetPixel(xCenter + y - 1, yCenter + x - 1, m_color);
	m_pImage->SetPixel(xCenter + y - 1, yCenter - x,     m_color);
	m_pImage->SetPixel(xCenter - y,     yCenter + x - 1, m_color);
	m_pImage->SetPixel(xCenter - y,     yCenter - x,     m_color);
}

void CCircleShape::_CircleFill(int xCenter, int yCenter, int x, int y)
{
	FillLine(xCenter - x, xCenter + x, yCenter + y - 1);
	FillLine(xCenter - x, xCenter + x, yCenter - y);
	FillLine(xCenter - y, xCenter + y, yCenter + x - 1);
	FillLine(xCenter - y, xCenter + y, yCenter - x);
}
