#include "StdAfx.h"
#include "ZXml.h"

int CZXml::ProcessLSquare(byte *pBuff ,int nLength)
{
	m_vecParen.push_back(LS_SQUARE);

	m_vecState.push_back(m_byState);
	m_byState = LS_SQUARE;

	m_byStatus = LS_BEGIN;
	return 1;
}

int CZXml::ProcessRSquare(byte *pBuff ,int nLength)
{
	//¿®∫≈≤ª∆•≈‰
	if (m_vecParen.back() != LS_SQUARE)
		return -1;
	m_vecParen.pop_back();

	m_byState = m_vecState.back();
	m_vecState.pop_back();

	m_byStatus = LS_BEGIN;

	//¿®∫≈∆•≈‰Ω·À„

	m_szString.clear();

	return 1;
}
