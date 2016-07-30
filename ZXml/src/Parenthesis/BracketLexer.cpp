#include "StdAfx.h"
#include "ZXml.h"

int CZXml::ProcessLBracket(byte *pBuff ,int nLength)
{
	m_vecParen.push_back(LS_BRACKET);
	m_byStatus = LS_BEGIN;
	return 1;
}

int CZXml::ProcessRBracket(byte *pBuff ,int nLength)
{
	//¿®∫≈≤ª∆•≈‰
	if (m_vecParen.back() != LS_BRACKET)
		return -1;
	m_vecParen.pop_back();

	m_byStatus = LS_BEGIN;
	return 1;
}
