#include "StdAfx.h"
#include "SolidFill.h"

CSolidFill::CSolidFill() : CFill()
{
}

CSolidFill::~CSolidFill()
{
}

void CSolidFill::FloodFill(CCanvas *pCanvas)
{
	m_pImage = pCanvas->GetImage();
	m_nOldColor = m_pImage->GetPixel(x0, y0);
	m_nNewColor = g_pZ2pho->GetColor();

	//颜色填充不进行填充
	if (m_nOldColor == m_nNewColor)
		return;

	//将种子(x0, y0)所在的区段入栈
	m_stack.clear();
	_PushSeed(x0, y0);

	while (!m_stack.empty())
	{
		m_span = m_stack.back();
		m_stack.pop_back();

		//循环处理发现的新区段
		while (_ScanLine(m_span.y + m_span.nUpDown, m_span.nUpDown));
	}

	g_pZ2coo->Redraw(pCanvas);
}

void CSolidFill::_PushSeed(int x0, int y0)
{
	//向右扫描并填充
	int i = x0;
	while (i < m_pImage->Width() && m_pImage->GetPixel(i, y0) == m_nOldColor)
	{
		m_pImage->SetPixel(i, y0, m_nNewColor);
		i++;
	}
	m_span.xRight = i - 1;
	//向左扫描并填充
	i = x0 - 1;
	while (i >= 0 && m_pImage->GetPixel(i, y0) == m_nOldColor)
	{
		m_pImage->SetPixel(i, y0, m_nNewColor);
		i--;
	}
	m_span.xLeft = i + 1;
	m_span.y = y0;

	m_span.nUpDown = 1;
	m_stack.push_back(m_span);
	m_span.nUpDown = -1;
	m_stack.push_back(m_span);
}

void CSolidFill::_PushSpan(int xLeft, int xRight, int y, int nUpDown)
{
	Span tNewSpan;
	tNewSpan.xLeft = xLeft;
	tNewSpan.xRight = xRight;
	tNewSpan.y = y;
	tNewSpan.nUpDown = nUpDown;
	m_stack.push_back(tNewSpan);
}

bool CSolidFill::_ScanLine(int y, int nUpDown)
{
	int xLeft;

	int nSpanLeft = m_span.xLeft; //记录区段的左边界
	int nSpanRight = m_span.xRight; //记录区段的右边界
	bool bNewSpan = false;
	bool bLeftEndSet = false; //bLeftEndSet = true表示左边界确定
	bool bSpanNeedFill;

	if (y < 0 || y >= m_pImage->Height())
		return false;

	int i = m_span.xLeft - 1;
	if (m_pImage->GetPixel(m_span.xLeft, y) == m_nOldColor)
	{
		while (i >= 0 && m_pImage->GetPixel(i, y) == m_nOldColor)
		{
			m_pImage->SetPixel(i, y, m_nNewColor); //向左填充
			i--;
		}
	}
	if (i != m_span.xLeft - 1) //确定区段左边界
	{
		bLeftEndSet = true;
		xLeft = i + 1;
	}
	i = m_span.xLeft;
	while (i <= nSpanRight)
	{
		bSpanNeedFill = false;
		while (i < m_pImage->Width() && m_pImage->GetPixel(i, y) == m_nOldColor)
		{
			if (!bSpanNeedFill)
			{
				bSpanNeedFill = true;
				if (!bLeftEndSet)
				{
					bLeftEndSet = true;
					xLeft = i;
				}
			}
			m_pImage->SetPixel(i, y, m_nNewColor); //向右填充
			i++;
		}
		if (bSpanNeedFill)
		{
			if (bNewSpan == false)
			{
				bNewSpan = true;

				//新发现的第一个区段不入栈
				m_span.y = y;
				m_span.xLeft = xLeft;
				m_span.xRight = i - 1;
				m_span.nUpDown = nUpDown;

				if (m_span.xLeft < nSpanLeft - 1) //出现反向回溯区
					_PushSpan(xLeft, nSpanLeft - 2, y, -nUpDown);

				if (m_span.xRight > nSpanRight + 1) //出现反向回溯区
					_PushSpan(nSpanRight + 2, i - 1, y, -nUpDown);
			}
			else
			{
				_PushSpan(xLeft, i - 1, y, nUpDown); //将新区段压入栈

				if (i - 1 > nSpanRight + 1) //出现反向回溯区
					_PushSpan(nSpanRight + 2, i - 1, y, -nUpDown);
			}
			bLeftEndSet = false;
			bSpanNeedFill = false;
		}

		while (i <= nSpanRight && m_pImage->GetPixel(i, y) != m_nOldColor)
			i++;
	}

	return bNewSpan;
}
