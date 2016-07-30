#include "StdAfx.h"

CContainer::CContainer() : CObject()
{
	m_pHead = NULL;
}

CContainer::CContainer(CPoint xDrawPos, CSize xSize, int nDepth)
				: CObject(xDrawPos, xSize, nDepth)
{
	m_pHead = NULL;
}

CContainer::~CContainer()
{
	delete m_pHead;
	m_pHead = NULL;
}

bool CContainer::Initial()
{
	m_pHead = new CObject();
	if (!m_pHead || !m_pHead->Initial())
		return false;

	return true;
}

bool CContainer::Release()
{
	delete this;
	return true;
}

inline void CContainer::SetDrawPos(int x, int y)
{
	CRect rect;
	GetRect(&rect);
	//重置绘制点
	m_xDrawPos.x = x;
	m_xDrawPos.y = y;
	//设置偏移
	_UpdateOffset(m_xOffset);
	g_pZ2coo->Redraw(this, rect);
}

void CContainer::Offset(int x, int y)
{
	CRect rect;
	GetRect(&rect);
	//重置绘制点
	m_xDrawPos.x += x;
	m_xDrawPos.y += y;
	//设置偏移
	_UpdateOffset(m_xOffset);
	g_pZ2coo->Redraw(this, rect);
}

void CContainer::_UpdateOffset(CPoint point)
{
	//自己的父偏移量
	CObject::_UpdateOffset(point);

	CObject *pNode = m_pHead;
	while (pNode = pNode->m_pNext)
	{
		//深度偏移
		pNode->_UpdateOffset(m_xOffset + m_xDrawPos);
	}
}

void CContainer::Draw(CImage *pImage)
{
	if (m_pHead->m_pNext)
		m_pHead->m_pNext->RevDraw(pImage);
}

void CContainer::Draw(CImage *pImage, const CRect &rect)
{
	if (m_pHead->m_pNext)
		m_pHead->m_pNext->RevDraw(pImage, rect);
}

void CContainer::AddObject(CObject *pObject)
{
	if (pObject == NULL)
		return;
	_DelObject(pObject);
	_AddObject(pObject);
	pObject->m_pParent = this;

	//深度调整
	pObject->_UpdateOffset(m_xOffset + m_xDrawPos);
	//发送重绘消息
	g_pZ2coo->Redraw(pObject);
}

void CContainer::DelObject(CObject *pObject)
{
	if (pObject == NULL)
		return;
	_DelObject(pObject);
	//发送重绘消息
	g_pZ2coo->Redraw(pObject);
}

bool CContainer::Including(CObject *pObject)
{
	CObject *pNode = m_pHead;
	//查找目标
	while (pNode = pNode->m_pNext)
	{
		if (pNode == pObject)
			return true;
	}
	return false;
}

CObject* CContainer::FindObject(const CPoint& point)
{
	CObject *pObject, *pNode = m_pHead;
	while (pNode = pNode->m_pNext)
	{
		//深度查找
		if (pNode->IsExist() && pNode->PtIn(point))
		{
			if (pObject = pNode->FindObject(point))
				return pObject;
		}
	}
	return NULL;
}

CObject* CContainer::FindWheel(const CPoint& point)
{
	CObject *pObject, *pNode = m_pHead;
	while (pNode = pNode->m_pNext)
	{
		//深度查找
		if (pNode->IsExist() && pNode->PtIn(point))
		{
			if (pNode->IsEnableAction(MA_MOUSEWHEEL))
				return pNode;

			if (pObject = pNode->FindWheel(point))
				return pObject;
		}
	}
	return NULL;
}

CObject* CContainer::FindExcept(const CPoint& point, CObject *pExcept)
{
	CObject *pObject, *pNode = m_pHead;
	while (pNode = pNode->m_pNext)
	{
		//本身是排除对象
		if (pNode == pExcept)
			continue;

		//深度查找
		if (pNode->IsExist() && pNode->PtIn(point))
		{
			if (pObject = pNode->FindObject(point))
				return pObject;
		}
	}
	return NULL;
}

void CContainer::_AddObject(CObject *pObject)
{
	int depth = pObject->GetDepth();
	CObject *pNode = m_pHead;
	//按深度由小到大排队
	while (pNode->m_pNext)
	{
		if (pNode->m_pNext->GetDepth() >= depth)
		{
			pObject->m_pNext = pNode->m_pNext;
			pNode->m_pNext = pObject;
			return;
		}
		pNode = pNode->m_pNext;
	}
	pNode->m_pNext = pObject;
}

void CContainer::_DelObject(CObject *pObject)
{
	CObject *pNode = m_pHead;
	//查找目标并删除
	while (pNode->m_pNext)
	{
		if (pNode->m_pNext == pObject)
		{
			pNode->m_pNext = pObject->m_pNext;
			pObject->m_pNext = NULL;
			break;
		}
		pNode = pNode->m_pNext;
	}
}
