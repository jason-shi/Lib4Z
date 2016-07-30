#include "StdAfx.h"

CEllipseShape::CEllipseShape(int xCenter, int yCenter, int Rx, int Ry)
				: xCenter(xCenter), yCenter(yCenter), Rx(Rx), Ry(Ry)
{
}

CEllipseShape::~CEllipseShape()
{
}

void CEllipseShape::_Draw(CCanvas *pCanvas)
{
	int Rx2 = Rx * Rx, Ry2 = Ry * Ry;
	int twoRx2 = 2 * Rx2, twoRy2 = 2 * Ry2;
	int p;
	int x = 0, y = Ry;
	int px = 0, py = twoRx2 * y;

	//plot first set of points
	_EllipsePlot(xCenter, yCenter, x, y);

	//Region 1
	p = ROUND(Ry2 - Rx2 * Ry + 0.25 * Rx2);
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		_EllipsePlot(xCenter, yCenter, x, y);
	}

	//Region 2
	p = ROUND(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
	while (y > 0)
	{
		y--;
		py -= twoRx2;
		if (p > 0)
			p += Rx2 - py;
		else
		{
			x++;
			px += twoRy2;
			p += Rx2 - py + px;
		}
		_EllipsePlot(xCenter, yCenter, x, y);
	}
	g_pZ2coo->Redraw(pCanvas);
}

void CEllipseShape::_Fill(CCanvas *pCanvas)
{
	int Rx2 = Rx * Rx, Ry2 = Ry * Ry;
	int twoRx2 = 2 * Rx2, twoRy2 = 2 * Ry2;
	int p;
	int x = 0, y = Ry;
	int px = 0, py = twoRx2 * y;

	//fill first set of points
	_EllipseFill(xCenter, yCenter, x, y);

	//Region 1
	p = ROUND(Ry2 - Rx2 * Ry + 0.25 * Rx2);
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		_EllipseFill(xCenter, yCenter, x, y);
	}

	//Region 2
	p = ROUND(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
	while (y > 0)
	{
		y--;
		py -= twoRx2;
		if (p > 0)
			p += Rx2 - py;
		else
		{
			x++;
			px += twoRy2;
			p += Rx2 - py + px;
		}
		_EllipseFill(xCenter, yCenter, x, y);
	}
	g_pZ2coo->Redraw(pCanvas);
}

void CEllipseShape::_EllipsePlot(int xCenter, int yCenter, int x, int y)
{
	m_pImage->SetPixel(xCenter + x - 1, yCenter + y - 1, m_color);
	m_pImage->SetPixel(xCenter + x - 1, yCenter - y,     m_color);
	m_pImage->SetPixel(xCenter - x,     yCenter + y - 1, m_color);
	m_pImage->SetPixel(xCenter - x,     yCenter - y,     m_color);
}

void CEllipseShape::_EllipseFill(int xCenter, int yCenter, int x, int y)
{
	FillLine(xCenter - x, xCenter + x, yCenter + y - 1);
	FillLine(xCenter - x, xCenter + x, yCenter - y);
}
