#include "StdAfx.h"
#include "Bezier2D.h"
#include "Bezier3D.h"
#include "BezierND.h"
#include "BezierCurve.h"

CBezierCurve::CBezierCurve(int nDegree, int nCount, LPPOINT lpPoints, bool *lpFlags, COLORREF *lpColors, bool bClose)
{
	m_nDegree = nDegree;
	m_nCount = nCount;
	m_lpPoints = lpPoints;
	m_lpFlags = lpFlags;
	m_lpColors = lpColors;
	m_bClose = bClose;
}

CBezierCurve::~CBezierCurve()
{
}

void CBezierCurve::Draw(CCanvas *pCanvas)
{
	switch (m_nDegree)
	{
	case 2:
		_Draw2D(pCanvas);
		break;
	case 3:
		_Draw3D(pCanvas);
		break;
	default:
		_DrawND(pCanvas);
		break;
	}
}

void CBezierCurve::_Draw2D(CCanvas *pCanvas)
{
	split_points[2] = m_lpPoints[0];

	//闭合时增加一次，闭合时的控制终点是P0
	for (int i = 1, j; i < m_nCount + m_bClose; i++)
	{
		j = i == m_nCount ? 0 : i;
		if (m_lpFlags[j])
		{
			//绘制Line(P0, P1)
			CLineShape tLineShape(split_points[2].x, split_points[2].y, m_lpPoints[j].x, m_lpPoints[j].y);
			tLineShape.SetColor(m_lpColors[i - 1]);
			tLineShape.Draw(pCanvas);

			split_points[2] = m_lpPoints[j];
		}
		else
		{
			//可以确保i < m_nCount
			split_points[0] = split_points[2];
			split_points[1] = m_lpPoints[i];

			j = i + 1 == m_nCount ? 0 : i + 1;
			if (m_lpFlags[j])
			{
				//绘制Curve(P0, P1, P2)
				split_points[2] = m_lpPoints[j];
				i++;
			}
			else
			{
				//绘制Curve(P0, P1, mid(P1P2))
				split_points[2].x = (m_lpPoints[i].x + m_lpPoints[j].x) / 2;
				split_points[2].y = (m_lpPoints[i].y + m_lpPoints[j].y) / 2;
			}
			CBezier2D tBezier(3, split_points);
			tBezier.SetColor(m_lpColors[i - 1]);
			tBezier.Draw(pCanvas);
		}
	}
}

void CBezierCurve::_Draw3D(CCanvas *pCanvas)
{
	split_points[3] = m_lpPoints[0];

	//初始化中点与四分之一点
	_UpdateCQ(0);

	//闭合时增加一次，闭合时的控制终点是P0
	for (int i = 1, j; i < m_nCount + m_bClose; i++)
	{
		j = i == m_nCount ? 0 : i;
		if (m_lpFlags[j])
		{
			if (m_lpFlags[i - 1])
			{
				//绘制Line(P0, P1)
				CLineShape tLineShape(split_points[3].x, split_points[3].y, m_lpPoints[j].x, m_lpPoints[j].y);
				tLineShape.SetColor(m_lpColors[i - 1]);
				tLineShape.Draw(pCanvas);

				split_points[3] = m_lpPoints[j];

				//更新中点与四分之一点
				_UpdateCQ(i);
				continue;
			}
			else
			{
				split_points[0] = split_points[3];
				split_points[1] = tQuarter;
				split_points[2].x = (tCenter.x + m_lpPoints[j].x) / 2;
				split_points[2].y = (tCenter.y + m_lpPoints[j].y) / 2;

				//更新中点与四分之一点
				_UpdateCQ(i);

				split_points[3] = m_lpPoints[j];
			}
		}
		else
		{
			split_points[0] = split_points[3];
			split_points[1] = tQuarter;
			split_points[2].x = (tCenter.x + m_lpPoints[i].x) / 2;
			split_points[2].y = (tCenter.y + m_lpPoints[i].y) / 2;

			//更新中点与四分之一点
			_UpdateCQ(i);

			split_points[3].x = (tQuarter.x + split_points[2].x) / 2;
			split_points[3].y = (tQuarter.y + split_points[2].y) / 2;
		}
		CBezier3D tBezier(4, split_points);
		tBezier.SetColor(m_lpColors[i - 1]);
		tBezier.Draw(pCanvas);
	}
}

void CBezierCurve::_DrawND(CCanvas *pCanvas)
{
	CBezierND tBezier(m_nCount, m_lpPoints);
	tBezier.SetColor(m_lpColors[0]);
	tBezier.Draw(pCanvas);
}

void CBezierCurve::_UpdateCQ(int i)
{
	if (i < m_nCount)
	{
		int j = i + 1 == m_nCount ? 0 : i + 1;

		//初始化中点与四分之一点
		tCenter.x = (m_lpPoints[i].x + m_lpPoints[j].x) / 2;
		tCenter.y = (m_lpPoints[i].y + m_lpPoints[j].y) / 2;
		tQuarter.x = (tCenter.x + m_lpPoints[i].x) / 2;
		tQuarter.y = (tCenter.y + m_lpPoints[i].y) / 2;
	}
}
