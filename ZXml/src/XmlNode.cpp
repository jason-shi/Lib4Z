#include "StdAfx.h"
#include "XmlNode.h"

using namespace std;

XmlNode::XmlNode(byte byType)
{
	m_byType = byType;
	m_nNumber = 0;
	m_pNext = NULL;
	m_pBaby = NULL;
	m_pAttr = NULL;
}

XmlNode::~XmlNode()
{
	XmlNode *pBaby = m_pBaby;
	while (pBaby)
	{
		XmlNode *pTemp = pBaby->m_pNext;
		delete pBaby;
		pBaby = pTemp;
	}

	XmlNode *pAttr = m_pAttr;
	while (pAttr)
	{
		XmlNode *pTemp = pAttr->m_pNext;
		delete pAttr;
		pAttr = pTemp;
	}
}

XmlNode *XmlNode::FindBaby(const string& szKey)
{
	XmlNode *pNode = m_pBaby;
	while (pNode)
	{
		if (pNode->m_szKey == szKey)
			return pNode;
		pNode = pNode->m_pNext;
	}
	return NULL;
}

void XmlNode::PushBaby(XmlNode *pBaby)
{
	XmlNode *pNode = m_pBaby;
	while (pNode && pNode->m_pNext)
		pNode = pNode->m_pNext;

	if (!pNode)
	{
		m_pBaby = pBaby;
		return;
	}
	pNode->m_pNext = pBaby;
}

XmlNode *XmlNode::FindAttr(const string& szKey)
{
	XmlNode *pNode = m_pAttr;
	while (pNode)
	{
		if (pNode->m_szKey == szKey)
			return pNode;
		pNode = pNode->m_pNext;
	}
	return NULL;
}

void XmlNode::PushAttr(XmlNode *pAttr)
{
	XmlNode *pNode = m_pAttr;
	while (pNode && pNode->m_pAttr)
		pNode = pNode->m_pAttr;

	if (!pNode)
	{
		m_pAttr = pAttr;
		return;
	}
	pNode->m_pAttr = pAttr;
}
