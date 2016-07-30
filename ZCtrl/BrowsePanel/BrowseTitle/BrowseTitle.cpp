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

	//�������һ���λ��
	int nItem = m_vecItem.size() - 1;
	_AdjustFollow(nItem, _GetDrawPos(nItem));

	AddObject(pObject);
}

void CBrowseTitle::InsertItem(int nItem, CObject *pObject)
{
	VECITEM::iterator iter = m_vecItem.begin();
	iter += nItem;
	m_vecItem.insert(iter, pObject);

	//�Ӳ���㿪ʼ����λ��
	_AdjustFollow(nItem, _GetDrawPos(nItem));

	AddObject(pObject);
}

void CBrowseTitle::RemoveItem(int nItem)
{
	VECITEM::iterator iter = m_vecItem.begin();
	iter += nItem;
	m_vecItem.erase(iter);

	//��ɾ���㿪ʼ����λ��
	_AdjustFollow(nItem, _GetDrawPos(nItem));

	DelObject(*iter);
}

void CBrowseTitle::AdjustWidth(int nItem, int nWidth)
{
	CObject *pObject = m_vecItem[nItem];

	//��ǰItem��λ�ò���
	pObject->SetSize(nWidth, pObject->GetSize().cy);
	_AdjustFollow(nItem, pObject->GetDrawPos());

	if (OnAdjustWidth)
		OnAdjustWidth(this, nItem, nWidth);
}

CPoint CBrowseTitle::_GetDrawPos(int nItem)
{
	if (nItem == 0)
		return CPoint(0, 0);

	//�����һ��Item����������
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

	//ȥ�����һ��Ŀ��
	if (!m_vecItem.empty())
		nTotalWidth -= m_vecItem.back()->GetSize().cx;

	//���ϴ��ڵĿ��
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
	//��ýӽ�����
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
	int nWidth1 = 0; //ǰi��֮��
	int nWidth2 = 0; //ǰi + 1��֮��
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

	//������Ŀƫ��
	VECITEM::iterator iter;
	for (iter = m_vecItem.begin(); iter != m_vecItem.end(); iter++)
		(*iter)->Offset(-xDrawPos);
}
