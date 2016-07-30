#include "StdAfx.h"
#include "BezierBase.h"

CBezierBase::CBezierBase(int nCount, LPPOINT lpPoints) : CCurve()
{
	m_nCount = nCount;
	m_lpPoints = lpPoints;

	for (int i = 0; i < m_nCount; i++)
	{
		m_span.ptx[i] = lpPoints[i].x << 16;
		m_span.pty[i] = lpPoints[i].y << 16;
	}

	m_stack.clear();
	m_stack.push_back(m_span);
}

CBezierBase::~CBezierBase()
{
}

void CBezierBase::Draw(CCanvas *pCanvas)
{
	int x1 = m_span.ptx[0] >> 16;
	int y1 = m_span.pty[0] >> 16;

	while (!m_stack.empty())
	{
		m_span = m_stack.back();
		m_stack.pop_back();

		//循环压入右半部分直到分割终止条件
		while (!CheckTermination(m_span.ptx, m_span.pty))
			DeCasteljau(m_span.ptx, m_span.pty);

		//用线段连接
		int x2 = ROUND(m_span.ptx[m_nCount - 1] / 65536.0);
		int y2 = ROUND(m_span.pty[m_nCount - 1] / 65536.0);

		CLineShape tLineShape(x1, y1, x2, y2);
		tLineShape.SetColor(m_color);
		tLineShape.Draw(pCanvas);

		x1 = x2;
		y1 = y2;
	}

	pCanvas->GetImage()->SetPixel(x1, y1, m_color);
}
