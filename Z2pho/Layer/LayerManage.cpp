#include "StdAfx.h"
#include "LayerManage.h"

CLayerManage::CLayerManage()
{
	m_pHead = new CLayer(NULL);
}

CLayerManage::~CLayerManage()
{
	delete m_pHead;
}

void CLayerManage::InsertLayer(CLayer *pCursor, CLayer *pLayer)
{
	pLayer->m_pNext = pCursor->m_pNext;
	pCursor->m_pNext = pLayer;
}

void CLayerManage::DeleteLayer(CLayer *pLayer)
{
	CLayer *pNode = m_pHead;
	while (pNode->m_pNext)
	{
		if (pNode->m_pNext == pLayer)
			break;
		pNode = pNode->m_pNext;
	}
	//从链表中脱链
	pNode->m_pNext = pLayer->m_pNext;
	pLayer->m_pNext = NULL;
}

void CLayerManage::BlendLayer(CCanvas *pCanvas, CLayer *pLayer)
{
	CImage *pImage = pLayer->GetImage();

	//图象区域
	CRect rect;
	rect.left = 0;
	rect.right = pImage->Width();
	rect.top = 0;
	rect.bottom = pImage->Height();

	pLayer->ImageBlend(pCanvas->GetImage(), pImage, rect, pLayer->GetDrawPos());
}

void CLayerManage::BlendAllLayer(CCanvas *pCanvas)
{
	CLayer *pNode = m_pHead;
	while (pNode = pNode->m_pNext)
	{
		if (pNode->GetVisible())
			BlendLayer(pCanvas, pNode);
	}
}
