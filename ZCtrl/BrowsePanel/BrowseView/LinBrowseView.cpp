#include "StdAfx.h"
#include "LinBrowseView.h"

CLinBrowseView::CLinBrowseView(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
					: CBrowseView(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	m_pBrowseBar = NULL;
	m_nCapacity = 0;

	m_nBrowseLine = 0;
	m_nBrowseItem = 0;
}

CLinBrowseView::~CLinBrowseView()
{
	for (int i = 0; i < m_nCapacity; i++)
	{
		delete m_pBrowseBar[i];
		m_pBrowseBar[i] = NULL;
	}
	delete m_pBrowseBar;
	m_pBrowseBar = NULL;
}

bool CLinBrowseView::InitBrowseBar(int nCapacity, CImage *pImage, CPoint xSrcPos, CSize xSize, int nDepth)
{
	m_nCapacity = nCapacity;

	m_pBrowseBar = new CBrowseBar *[m_nCapacity];
	if (!m_pBrowseBar)
		return false;

	//创建并添加浏览条
	for (int i = 0; i < m_nCapacity; i++)
	{
		m_pBrowseBar[i] = CreateBrowseBar(pImage, xSrcPos, xSize, nDepth);
		if (!m_pBrowseBar[i] || !m_pBrowseBar[i]->Initial() || !m_pBrowseBar[i]->CreateItems())
			return false;

		AddObject(m_pBrowseBar[i]);
	}
	return true;
}

void CLinBrowseView::ShowBrowseView()
{
#define Bar			 m_pBrowseBar[nBarCursor] //唯一引用方式

	int nTotalLine = m_pBrowseData->GetTotalLine();
	int nDataCursor = m_pBrowseData->GetDataCursor();

	int nTotalHeight = 0;
	int nBarCursor = m_nBrowseLine % m_nCapacity;

	//隐藏所有浏览条
	for (int i = 0; i < m_nCapacity; i++)
		m_pBrowseBar[i]->Show(false);

	if (m_nBrowseLine < nDataCursor)
	{
		//显示[m_nBrowseLine, m_nDataCursor)
		for (int i = 0; i < nDataCursor - m_nBrowseLine; i++)
		{
			if (++nBarCursor == m_nCapacity)
				nBarCursor = 0;
			Bar->_Load(m_pBrowseData, m_nBrowseLine + i);
			Bar->SetDrawPos(0, nTotalHeight);

			//高度累加
			if ((nTotalHeight += Bar->GetSize().cy) >= m_xSize.cy)
				goto Redraw;
		}
	}
	else
	{
		//显示[m_nBrowseLine, nTotalLine)
		for (int i = 0; i < nTotalLine - m_nBrowseLine; i++)
		{
			if (++nBarCursor == m_nCapacity)
				nBarCursor = 0;
			Bar->_Load(m_pBrowseData, m_nBrowseLine + i);
			Bar->SetDrawPos(0, nTotalHeight);

			//高度累加
			if ((nTotalHeight += Bar->GetSize().cy) >= m_xSize.cy)
				goto Redraw;
		}
		//继续显示[0, m_nDataCursor)
		for (int i = 0; i < nDataCursor; i++)
		{
			if (++nBarCursor == m_nCapacity)
				nBarCursor = 0;
			Bar->_Load(m_pBrowseData, i);
			Bar->SetDrawPos(0, nTotalHeight);

			//高度累加
			if ((nTotalHeight += Bar->GetSize().cy) >= m_xSize.cy)
				goto Redraw;
		}
	}
Redraw:
	g_pZ2coo->Redraw(this);

#undef Bar
}

void CLinBrowseView::UpdateView()
{
	int nTotalLine = m_pBrowseData->GetTotalLine();
	int nCycleLine = m_pBrowseData->GetCycleLine();
	int nDataCursor = m_pBrowseData->GetDataCursor();

	if (IsAutoScroll())
	{
		if (m_pBrowseData->GetRealHeight(m_nBrowseLine) > m_xSize.cy)
		{
			//窗口容纳不下，需要滚动
			m_nBrowseLine = m_pBrowseData->GetBrowseLine(m_xSize.cy);
		}
	}
	else
	{
		if (nTotalLine == nCycleLine && nCycleLine != 0)
		{
			//数据发生覆盖，必须滚动一行
			if (nDataCursor == (m_nBrowseLine + 1) % nCycleLine)
				m_nBrowseLine = nDataCursor;
		}
	}

	ShowBrowseView();
}

int CLinBrowseView::GetFrontWidth() const
{
	return m_pBrowseBar[0]->GetFrontWidth(m_nBrowseItem);
}

int CLinBrowseView::GetFrontHeight() const
{
	return m_pBrowseData->GetFrontHeight(m_nBrowseLine);
}

int CLinBrowseView::GetFieldWidth() const
{
	return m_xSize.cx;
}

int CLinBrowseView::GetFieldHeight() const
{
	return m_pBrowseData->GetFieldHeight(m_nBrowseLine, m_xSize.cy, m_bAutoScroll);
}

int CLinBrowseView::GetTotalWidth() const
{
	return m_pBrowseBar[0]->GetTotalWidth(m_xSize.cx);
}

int CLinBrowseView::GetTotalHeight() const
{
	return m_pBrowseData->GetTotalHeight(m_xSize.cy, m_bAutoScroll);
}

void CLinBrowseView::OnVScrollWindow(int nDelta)
{
	int nTotalLine = m_pBrowseData->GetTotalLine();
	int nCycleLine = m_pBrowseData->GetCycleLine();
	int nDataCursor = m_pBrowseData->GetDataCursor();

	if (!m_bAutoScroll)
	{
		//拉到屏显只剩一行
		if (m_nBrowseLine == nDataCursor - 1)
			m_bAutoScroll = true;
	}
	if (IsAutoScroll())
	{
		//只要是向上拉动
		if (nDelta < 0)
			m_bAutoScroll = false;
	}

	if (nTotalLine < nCycleLine || nCycleLine == 0)
	{
		m_nBrowseLine += nDelta;
	
		m_nBrowseLine = min(m_nBrowseLine, nTotalLine - 1);
		m_nBrowseLine = max(0, m_nBrowseLine);
	}
	else
	{
		//已经发生循环
		if (m_nBrowseLine < nDataCursor)
		{
			m_nBrowseLine += nDelta;

			if (nDelta > 0)
				m_nBrowseLine = min(m_nBrowseLine, nDataCursor - 1);
			else
			{
				if (m_nBrowseLine < 0)
				{
					m_nBrowseLine += nTotalLine;
					m_nBrowseLine = max(m_nBrowseLine, nDataCursor);
				}
			}
		}
		else
		{
			m_nBrowseLine += nDelta;

			if (nDelta < 0)
				m_nBrowseLine = max(m_nBrowseLine, nDataCursor);
			else
			{
				if (m_nBrowseLine >= nTotalLine)
				{
					m_nBrowseLine -= nTotalLine;
					m_nBrowseLine = min(m_nBrowseLine, nDataCursor - 1);

					//因min(0, -1)导致的调整
					if (m_nBrowseLine == -1)
						m_nBrowseLine = nTotalLine - 1;
				}
			}
		}
	}

	ShowBrowseView();
}

void CLinBrowseView::OnHScrollWindow(int nDelta)
{
	m_nBrowseItem = m_pBrowseBar[0]->CalcItem(m_nBrowseItem, nDelta);

	//只更新水平位置
	for (int i = 0; i < m_nCapacity; i++)
		m_pBrowseBar[i]->UpdateBrowseBar(m_nBrowseItem);
}

void CLinBrowseView::OnVSlideWindow(int nFront, int nTotal)
{
	//因拉动产生的自动滚屏
	if (IsAutoScroll())
	{
		if (m_pBrowseData->GetRealHeight(m_nBrowseLine) > m_xSize.cy)
		{
			//窗口容纳不下，需要滚动
			m_nBrowseLine = m_pBrowseData->GetBrowseLine(m_xSize.cy);
		}
	}
	else
	{
		//获得接近的行
		int nFrontHeight = (float)nFront / nTotal * GetTotalHeight();
		m_nBrowseLine = m_pBrowseData->GetProximalLine(nFrontHeight);
	}
	ShowBrowseView();
}

void CLinBrowseView::OnHSlideWindow(int nFront, int nTotal)
{
	//获得接近的列
	float fFrontWidth = (float)nFront / nTotal * GetTotalWidth();
	m_nBrowseItem = m_pBrowseBar[0]->CalcItem(fFrontWidth);

	//只更新水平位置
	for (int i = 0; i < m_nCapacity; i++)
		m_pBrowseBar[i]->UpdateBrowseBar(m_nBrowseItem);
}

void CLinBrowseView::OnAdjustWidth(int nItem, int nWidth)
{
	for (int i = 0; i < m_nCapacity; i++)
	{
		//简单起见，非可见区域的也进行宽度调整
		m_pBrowseBar[i]->OnAdjustWidth(nItem, nWidth);
	}
}
