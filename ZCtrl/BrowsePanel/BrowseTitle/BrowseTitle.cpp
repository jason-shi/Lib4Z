#include "StdAfx.h"
#include "BrowseTitle.h"

CBrowseTitle::CBrowseTitle(CImage *pImage, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, CPoint(0, 0), xSrcPos, xSize, nDepth)
{
	m_nBrowseItem = 0;
}

CBrowseTitle::~CBrowseTitle()
{
	m_vecItem.clear();
}

void CBrowseTitle::AppendItem(CObject *pObject)
{
	m_vecItem.push_back(pObject);

	//调整最后一项的位置
	int nItem = m_vecItem.size() - 1;
	_AdjustFollow(nItem, _GetDrawPos(nItem));

	AddObject(pObject);
}

void CBrowseTitle::InsertItem(int nItem, CObject *pObject)
{
	VECITEM::iterator iter = m_vecItem.begin();
	iter += nItem;
	m_vecItem.insert(iter, pObject);

	//从插入点开始调整位置
	_AdjustFollow(nItem, _GetDrawPos(nItem));

	AddObject(pObject);
}

void CBrowseTitle::RemoveItem(int nItem)
{
	VECITEM::iterator iter = m_vecItem.begin();
	iter += nItem;
	m_vecItem.erase(iter);

	//从删除点开始调整位置
	_AdjustFollow(nItem, _GetDrawPos(nItem));

	DelObject(*iter);
}

void CBrowseTitle::AdjustWidth(int nItem, int nWidth)
{
	CObject *pObject = m_vecItem[nItem];

	//当前Item的位置不变
	pObject->SetSize(nWidth, pObject->GetSize().cy);
	_AdjustFollow(nItem, pObject->GetDrawPos());

	if (OnAdjustWidth)
		OnAdjustWidth(this, nItem, nWidth);
}

CPoint CBrowseTitle::_GetDrawPos(int nItem)
{
	if (nItem == 0)
		return CPoint(0, 0);

	//获得上一个Item的右上坐标
	CObject *pPrev = m_vecItem[nItem - 1];
	CPoint xDrawPos = pPrev->GetDrawPos();
	CSize xSize = pPrev->GetSize();
	return CPoint(xDrawPos.x + xSize.cx, xDrawPos.y);
}

void CBrowseTitle::_AdjustFollow(int nItem, CPoint xDrawPos)
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

int CBrowseTitle::GetFrontWidth(int nBrowseItem)
{
	int nFrontWidth = 0;

	VECITEM::iterator iter;
	VECITEM::iterator limit = m_vecItem.begin() + nBrowseItem;
	for (iter = m_vecItem.begin(); iter < limit; iter++)
		nFrontWidth += (*iter)->GetSize().cx;

	return nFrontWidth;
}

int CBrowseTitle::GetTotalWidth(int nViewWidth)
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

void CBrowseTitle::OnHScrollWindow(int nDelta)
{
	m_nBrowseItem = CalcItem(m_nBrowseItem, nDelta);

	UpdateBrowseTitle(m_nBrowseItem);
}

void CBrowseTitle::OnHSlideWindow(int nFront, int nTotal)
{
	//获得接近的列
	float fFrontWidth = (float)nFront / nTotal * m_xSize.cx;
	m_nBrowseItem = CalcItem(fFrontWidth);

	UpdateBrowseTitle(m_nBrowseItem);
}

int CBrowseTitle::CalcItem(int nBrowseItem, int nDelta)
{
	nBrowseItem += nDelta;

	if (nBrowseItem < 0)
		nBrowseItem = 0;
	if (nBrowseItem >= m_vecItem.size())
		nBrowseItem = m_vecItem.size() - 1;

	return nBrowseItem;
}

int CBrowseTitle::CalcItem(int nFrontWidth)
{
	int nWidth1 = 0; //前i项之和
	int nWidth2 = 0; //前i + 1项之和
	for (int i = 0; i < m_vecItem.size(); i++)
	{
		nWidth2 += m_vecItem[i]->GetSize().cx;
		if (nWidth2 > nFrontWidth)
			return nFrontWidth - nWidth1 < nWidth2 - nFrontWidth ? i : i + 1;

		nWidth1 += m_vecItem[i]->GetSize().cx;
	}
	return m_vecItem.size() - 1;
}

void CBrowseTitle::UpdateBrowseTitle(int nBrowseItem)
{
	CPoint xDrawPos = m_vecItem[nBrowseItem]->GetDrawPos();

	//所有项目偏移
	VECITEM::iterator iter;
	for (iter = m_vecItem.begin(); iter != m_vecItem.end(); iter++)
		(*iter)->Offset(-xDrawPos);
}
