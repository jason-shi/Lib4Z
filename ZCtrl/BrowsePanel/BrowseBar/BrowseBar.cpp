#include "StdAfx.h"
#include "BrowseBar.h"

CBrowseBar::CBrowseBar(CImage *pImage, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, CPoint(0, 0), xSrcPos, xSize, nDepth)
{
	m_nDataIndex = -1;
}

CBrowseBar::~CBrowseBar()
{
}

void CBrowseBar::_Load(CBrowseData *pBrowseData, int nDataIndex)
{
	if (nDataIndex != m_nDataIndex //对应的行数有变化
		|| pBrowseData->IsDataLost(nDataIndex)) //对应行的数据有变化
	{
		Load(pBrowseData, nDataIndex);
		m_nDataIndex = nDataIndex;

		pBrowseData->ClearDataLost(m_nDataIndex);

		//高度不受Load的影响
		m_xSize.cy = pBrowseData->GetBarHeight(m_nDataIndex);
	}
	m_bShow = true; //加载意味着显示
}

void CBrowseBar::AppendItem(CObject *pObject)
{
	m_vecItem.push_back(pObject);

	//调整最后一项的位置
	int nItem = m_vecItem.size() - 1;
	_AdjustFollow(nItem, _GetDrawPos(nItem));

	AddObject(pObject);
}

void CBrowseBar::InsertItem(int nItem, CObject *pObject)
{
	VECITEM::iterator iter = m_vecItem.begin();
	iter += nItem;
	m_vecItem.insert(iter, pObject);

	//从插入点开始调整位置
	_AdjustFollow(nItem, _GetDrawPos(nItem));

	AddObject(pObject);
}

void CBrowseBar::RemoveItem(int nItem)
{
	VECITEM::iterator iter = m_vecItem.begin();
	iter += nItem;
	m_vecItem.erase(iter);

	//从删除点开始调整位置
	_AdjustFollow(nItem, _GetDrawPos(nItem));

	DelObject(*iter);
}

void CBrowseBar::OnAdjustWidth(int nItem, int nWidth)
{
	CObject *pObject = m_vecItem[nItem];

	//当前Item的位置不变
	pObject->SetSize(nWidth, pObject->GetSize().cy);
	_AdjustFollow(nItem, pObject->GetDrawPos());
}

CPoint CBrowseBar::_GetDrawPos(int nItem)
{
	if (nItem == 0)
		return CPoint(0, 0);

	//获得上一个Item的右上坐标
	CObject *pPrev = m_vecItem[nItem - 1];
	CPoint xDrawPos = pPrev->GetDrawPos();
	CSize xSize = pPrev->GetSize();
	return CPoint(xDrawPos.x + xSize.cx, xDrawPos.y);
}

void CBrowseBar::_AdjustFollow(int nItem, CPoint xDrawPos)
{
	VECITEM::iterator iter = m_vecItem.begin();
	for (iter += nItem; iter != m_vecItem.end(); iter++)
	{
		if (xDrawPos.x >= m_xSize.cx)
			break;
		(*iter)->SetDrawPos(xDrawPos);
		xDrawPos.x += (*iter)->GetSize().cx;
	}
}

int CBrowseBar::GetFrontWidth(int nBrowseItem)
{
	int nFrontWidth = 0;

	VECITEM::iterator iter;
	VECITEM::iterator limit = m_vecItem.begin() + nBrowseItem;
	for (iter = m_vecItem.begin(); iter < limit; iter++)
		nFrontWidth += (*iter)->GetSize().cx;

	return nFrontWidth;
}

int CBrowseBar::GetTotalWidth(int nViewWidth)
{
	int nTotalWidth = 0;

	VECITEM::iterator iter;
	for (iter = m_vecItem.begin(); iter != m_vecItem.end(); iter++)
		nTotalWidth += (*iter)->GetSize().cx;

	//去除最后一项的宽度
	if (!m_vecItem.empty())
		nTotalWidth -= m_vecItem.back()->GetSize().cx;

	//加上窗口的宽度
	nTotalWidth += nViewWidth;

	return nTotalWidth;
}

int CBrowseBar::CalcItem(int nBrowseItem, int nDelta)
{
	nBrowseItem += nDelta;

	if (nBrowseItem < 0)
		nBrowseItem = 0;
	if (nBrowseItem >= m_vecItem.size())
		nBrowseItem = m_vecItem.size() - 1;

	return nBrowseItem;
}

int CBrowseBar::CalcItem(int nFrontWidth)
{
	int nWidth1 = 0; //前i项之和
	int nWidth2 = 0; //前i + 1项之和
	for (int i = 0; i < m_vecItem.size() - 1; i++)
	{
		nWidth2 += m_vecItem[i]->GetSize().cx;
		if (nWidth2 > nFrontWidth)
			return nFrontWidth - nWidth1 < nWidth2 - nFrontWidth ? i : i + 1;

		nWidth1 += m_vecItem[i]->GetSize().cx;
	}
	return m_vecItem.size() - 1;
}

void CBrowseBar::UpdateBrowseBar(int nBrowseItem)
{
	CPoint xDrawPos = m_vecItem[nBrowseItem]->GetDrawPos();

	//所有项目偏移
	VECITEM::iterator iter;
	for (iter = m_vecItem.begin(); iter != m_vecItem.end(); iter++)
		(*iter)->Offset(-xDrawPos);
}
