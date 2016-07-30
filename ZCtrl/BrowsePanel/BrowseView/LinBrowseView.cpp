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

	//��������������
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
#define Bar			 m_pBrowseBar[nBarCursor] //Ψһ���÷�ʽ

	int nTotalLine = m_pBrowseData->GetTotalLine();
	int nDataCursor = m_pBrowseData->GetDataCursor();

	int nTotalHeight = 0;
	int nBarCursor = m_nBrowseLine % m_nCapacity;

	//�������������
	for (int i = 0; i < m_nCapacity; i++)
		m_pBrowseBar[i]->Show(false);

	if (m_nBrowseLine < nDataCursor)
	{
		//��ʾ[m_nBrowseLine, m_nDataCursor)
		for (int i = 0; i < nDataCursor - m_nBrowseLine; i++)
		{
			if (++nBarCursor == m_nCapacity)
				nBarCursor = 0;
			Bar->_Load(m_pBrowseData, m_nBrowseLine + i);
			Bar->SetDrawPos(0, nTotalHeight);

			//�߶��ۼ�
			if ((nTotalHeight += Bar->GetSize().cy) >= m_xSize.cy)
				goto Redraw;
		}
	}
	else
	{
		//��ʾ[m_nBrowseLine, nTotalLine)
		for (int i = 0; i < nTotalLine - m_nBrowseLine; i++)
		{
			if (++nBarCursor == m_nCapacity)
				nBarCursor = 0;
			Bar->_Load(m_pBrowseData, m_nBrowseLine + i);
			Bar->SetDrawPos(0, nTotalHeight);

			//�߶��ۼ�
			if ((nTotalHeight += Bar->GetSize().cy) >= m_xSize.cy)
				goto Redraw;
		}
		//������ʾ[0, m_nDataCursor)
		for (int i = 0; i < nDataCursor; i++)
		{
			if (++nBarCursor == m_nCapacity)
				nBarCursor = 0;
			Bar->_Load(m_pBrowseData, i);
			Bar->SetDrawPos(0, nTotalHeight);

			//�߶��ۼ�
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
			//�������ɲ��£���Ҫ����
			m_nBrowseLine = m_pBrowseData->GetBrowseLine(m_xSize.cy);
		}
	}
	else
	{
		if (nTotalLine == nCycleLine && nCycleLine != 0)
		{
			//���ݷ������ǣ��������һ��
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
		//��������ֻʣһ��
		if (m_nBrowseLine == nDataCursor - 1)
			m_bAutoScroll = true;
	}
	if (IsAutoScroll())
	{
		//ֻҪ����������
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
		//�Ѿ�����ѭ��
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

					//��min(0, -1)���µĵ���
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

	//ֻ����ˮƽλ��
	for (int i = 0; i < m_nCapacity; i++)
		m_pBrowseBar[i]->UpdateBrowseBar(m_nBrowseItem);
}

void CLinBrowseView::OnVSlideWindow(int nFront, int nTotal)
{
	//�������������Զ�����
	if (IsAutoScroll())
	{
		if (m_pBrowseData->GetRealHeight(m_nBrowseLine) > m_xSize.cy)
		{
			//�������ɲ��£���Ҫ����
			m_nBrowseLine = m_pBrowseData->GetBrowseLine(m_xSize.cy);
		}
	}
	else
	{
		//��ýӽ�����
		int nFrontHeight = (float)nFront / nTotal * GetTotalHeight();
		m_nBrowseLine = m_pBrowseData->GetProximalLine(nFrontHeight);
	}
	ShowBrowseView();
}

void CLinBrowseView::OnHSlideWindow(int nFront, int nTotal)
{
	//��ýӽ�����
	float fFrontWidth = (float)nFront / nTotal * GetTotalWidth();
	m_nBrowseItem = m_pBrowseBar[0]->CalcItem(fFrontWidth);

	//ֻ����ˮƽλ��
	for (int i = 0; i < m_nCapacity; i++)
		m_pBrowseBar[i]->UpdateBrowseBar(m_nBrowseItem);
}

void CLinBrowseView::OnAdjustWidth(int nItem, int nWidth)
{
	for (int i = 0; i < m_nCapacity; i++)
	{
		//��������ǿɼ������Ҳ���п�ȵ���
		m_pBrowseBar[i]->OnAdjustWidth(nItem, nWidth);
	}
}
