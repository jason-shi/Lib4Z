#include "StdAfx.h"
#include "ZXml.h"

int CZXml::ProcessLBrace(byte *pBuff ,int nLength)
{
	XmlAttr *pXmlAttr = new XmlAttr(LN_ELEMENT);
	SwitchXmlNode(pXmlAttr);

	m_vecParen.push_back(LS_BRACE);

	m_vecState.push_back(m_byState);
	m_byState = LS_BRACE;

	m_byStatus = LS_BEGIN;
	return 1;
}

int CZXml::ProcessRBrace(byte *pBuff ,int nLength)
{
	//���Ų�ƥ��
	if (m_vecParen.back() != LS_BRACE)
		return -1;
	m_vecParen.pop_back();

	//��Ӧ{ A }������
	if (m_byState == LN_ELEMENT || m_byState == LS_NUMBER || m_byState == LS_STRING)
	{
		if (!PushXmlData())
			return -1;
		m_vecState.pop_back();
	}

	//״̬תΪITEM
	m_byState = LS_ARRAY;
	m_byStatus = LS_BEGIN;
	return 1;
}
