#include "StdAfx.h"
#include "BrowseData.h"

CBrowseData::CBrowseData(int nCycleLine) : m_nCycleLine(nCycleLine)
{
	m_nTotalLine = 0;
	m_nDataCursor = 0;

	m_nTotalHeight = 0;
}

CBrowseData::~CBrowseData()
{
}

void CBrowseData::OnRecvData(int nHeight)
{
	if (m_nTotalLine < m_nCycleLine || m_nCycleLine == 0)
	{
		m_vecDataLost.push_back(true);
		m_vecHeight.push_back(nHeight);

		m_nTotalHeight += nHeight;
		m_nTotalLine++;
	}
	else
	{
		//已经发生循环
		OnChangeData(m_nDataCursor, nHeight);
	}

	//到达循环终点
	if (++m_nDataCursor == m_nCycleLine)
		m_nDataCursor = 0;
}

void CBrowseData::OnChangeData(int nDataIndex, int nHeight)
{
	//标记数据更改
	m_vecDataLost[nDataIndex] = true;

	//高度变更
	int nOldHeight = m_vecHeight[nDataIndex];
	m_vecHeight[nDataIndex] = nHeight;

	m_nTotalHeight += nHeight - nOldHeight;
}

void CBrowseData::FlushData()
{
	OnFlushData(m_pBrowseView);
}

void CBrowseData::FlushData(void *pBrowseView)
{
	if (!pBrowseView)
		return;

	OnFlushData(pBrowseView);
}

int CBrowseData::GetBarHeight(int nDataIndex) const
{
	return m_vecHeight[nDataIndex];
}

int CBrowseData::GetFrontHeight(int nBrowseLine) const
{
	int nFrontHeight = 0;
	if (m_nTotalLine < m_nCycleLine || m_nCycleLine == 0)
	{
		for (int i = 0; i < nBrowseLine; i++)
			nFrontHeight += m_vecHeight[i];
	}
	else
	{
		//已经发生循环
		if (nBrowseLine < m_nDataCursor)
		{
			for (int i = m_nDataCursor; i < m_nTotalLine; i++)
				nFrontHeight += m_vecHeight[i];
			for (int i = 0; i < nBrowseLine; i++)
				nFrontHeight += m_vecHeight[i];
		}
		else
		{
			for (int i = m_nDataCursor; i < nBrowseLine; i++)
				nFrontHeight += m_vecHeight[i];
		}
	}
	return nFrontHeight;
}

int CBrowseData::GetRealHeight(int nBrowseLine) const
{
	int nHeight = 0;
	if (m_nTotalLine < m_nCycleLine || m_nCycleLine == 0)
	{
		for (int i = nBrowseLine; i < m_nDataCursor; i++)
			nHeight += m_vecHeight[i];
	}
	else
	{
		//已经发生循环
		if (nBrowseLine < m_nDataCursor)
		{
			for (int i = nBrowseLine; i < m_nDataCursor; i++)
				nHeight += m_vecHeight[i];
		}
		else
		{
			for (int i = nBrowseLine; i < m_nTotalLine; i++)
				nHeight += m_vecHeight[i];
			for (int i = 0; i < m_nDataCursor; i++)
				nHeight += m_vecHeight[i];
		}
	}
	return nHeight;
}

int CBrowseData::GetFieldHeight(int nBrowseLine, int nViewHeight, bool bAutoScroll) const
{
	if (bAutoScroll)
	{
		int nHeight = 0;
		if (m_nTotalLine < m_nCycleLine || m_nCycleLine == 0)
		{
			for (int i = nBrowseLine; i < m_nDataCursor; i++)
			{
				nHeight += m_vecHeight[i];
				if (nHeight >= nViewHeight)
					return nViewHeight;
			}
			return max(nHeight, nViewHeight);
		}
		else
		{
			//已经发生循环
			if (nBrowseLine < m_nDataCursor)
			{
				for (int i = nBrowseLine; i < m_nDataCursor; i++)
				{
					nHeight += m_vecHeight[i];
					if (nHeight >= nViewHeight)
						return nViewHeight;
				}
				return max(nHeight, nViewHeight);
			}
			else
			{
				for (int i = nBrowseLine; i < m_nTotalLine; i++)
				{
					nHeight += m_vecHeight[i];
					if (nHeight >= nViewHeight)
						return nViewHeight;
				}
				for (int i = 0; i < m_nDataCursor; i++)
				{
					nHeight += m_vecHeight[i];
					if (nHeight >= nViewHeight)
						return nViewHeight;
				}
				return max(nHeight, nViewHeight);
			}
		}
	}

	//视区高度
	return nViewHeight;
}

int CBrowseData::GetTotalHeight(int nViewHeight, bool bAutoScroll) const
{
	return m_nTotalHeight - _GetLastHeight() + nViewHeight;
}

int CBrowseData::GetProximalLine(int nFrontHeight) const
{
	int nHeight1 = 0; //前i项之和
	int nHeight2 = 0; //前i + 1项之和
	if (m_nTotalLine < m_nCycleLine || m_nCycleLine == 0)
	{
		for (int i = 0; i < m_nDataCursor - 1; i++)
		{
			nHeight2 += m_vecHeight[i];
			if (nHeight2 > nFrontHeight)
				return nFrontHeight - nHeight1 < nHeight2 - nFrontHeight ? i : i + 1;

			nHeight1 += m_vecHeight[i];
		}
		return max(0, m_nDataCursor - 1);
	}
	else
	{
		for (int i = m_nDataCursor, nLine = 0; nLine < m_nTotalLine - 1; i++, nLine++)
		{
			if (i == m_nTotalLine) //循环
				i = 0;

			nHeight2 += m_vecHeight[i];
			if (nHeight2 > nFrontHeight)
				return nFrontHeight - nHeight1 < nHeight2 - nFrontHeight ? i : (i + 1) % m_nTotalLine;

			nHeight1 += m_vecHeight[i];
		}
		return max(0, m_nDataCursor - 1);
	}
}

int CBrowseData::GetBrowseLine(int nViewHeight) const
{
	int nHeight = 0;
	if (m_nTotalLine < m_nCycleLine || m_nCycleLine == 0)
	{
		for (int i = m_nDataCursor - 1; i >= 0; i--)
		{
			nHeight += m_vecHeight[i];
			if (nHeight >= nViewHeight)
				return i;
		}
		return 0;
	}
	else
	{
		for (int i = m_nDataCursor - 1; i >= 0; i--)
		{
			nHeight += m_vecHeight[i];
			if (nHeight >= nViewHeight)
				return i;
		}
		for (int i = m_nTotalLine - 1; i >= m_nDataCursor; i--)
		{
			nHeight += m_vecHeight[i];
			if (nHeight >= nViewHeight)
				return i;
		}
		return m_nDataCursor;
	}
}

int CBrowseData::_GetLastHeight() const
{
	if (m_nDataCursor == 0)
	{
		if (m_nTotalLine < m_nCycleLine || m_nCycleLine == 0)
			return 0;

		return m_vecHeight[m_nTotalLine - 1];
	}
	return m_vecHeight[m_nDataCursor - 1];
}
