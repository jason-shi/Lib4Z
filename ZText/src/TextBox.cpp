#include "StdAfx.h"
#include "TextBox.h"

CTextBox::CTextBox(CPoint xDrawPos, CSize xSize, int nDepth)
			: CPanel(NULL, xDrawPos, xSize, nDepth)
{
	m_nCapacity = 16; //默认容量
	m_nTopLine = 0;
	m_nTopMargin = 0;

	m_pTextBar = NULL;
}

CTextBox::~CTextBox()
{
	for (int i = 0; i < m_nCapacity; i++)
	{
		delete m_pTextBar[i];
		m_pTextBar[i] = NULL;
	}
	delete m_pTextBar;
	m_pTextBar = NULL;

	VECLINETAG::iterator iter;
	for (iter = m_vecLineTag.begin(); iter != m_vecLineTag.end(); iter++)
		delete *iter;
}

bool CTextBox::Initial()
{
	m_pTextBar = new CTextBar *[m_nCapacity];
	if (!m_pTextBar)
		return false;

	for (int i = 0; i < m_nCapacity; i++)
	{
		m_pTextBar[i] = new CTextBar(false, 0, CPoint(0, 18 * i), CSize(640, 18), 0);
		AddObject(m_pTextBar[i]);
	}
	return true;
}

bool CTextBox::Release()
{
	CPanel::Release();

	for (int i = 0; i < m_nCapacity; i++)
	{
		delete m_pTextBar[i];
		m_pTextBar[i] = NULL;
	}
	delete m_pTextBar;
	m_pTextBar = NULL;

	VECLINETAG::iterator iter;
	for (iter = m_vecLineTag.begin(); iter != m_vecLineTag.end(); iter++)
		delete *iter;

	return true;
}

CLineTag *CTextBox::GetLineTag(int nLine)
{
	if (nLine >= m_vecLineTag.size())
		return NULL;

	return m_vecLineTag[nLine];
}

void CTextBox::ExtOutText(const TCHAR *lpText, int nLength)
{
	CLineTag *pLineTag = GetLineTag(0);
	pLineTag->Initial();

	int nTotalHeight = 0;
	for (int i = 0; i < 5; i++)
	{
		m_pTextBar[i]->SetDrawPos(0, nTotalHeight);
		nTotalHeight += m_pTextBar[i]->ExtOutText(lpText, nLength, pLineTag);
	}
}

void CTextBox::EnhOutText(const TCHAR *lpText, int nLength)
{
	CLineTag *pLineTag = GetLineTag(0);
	pLineTag->Initial();

	int nLine = m_nTopLine;
	int nTotalHeight = m_nTopMargin;

	while (nTotalHeight < m_xSize.cy)
	{
		m_pTextBar[nLine]->SetDrawPos(0, nTotalHeight);
		nTotalHeight += m_pTextBar[nLine]->EnhOutText(lpText, nLength, pLineTag);
	}
}

void CTextBox::EnhScanText(const TCHAR *lpText, int nLength)
{
	CLineTag *pScanLineTag = new CLineTag();
	pScanLineTag->Initial();

	CTextBar *pTextBar = new CTextBar(false, 0, CPoint(0, 0), CSize(m_xSize.cx, 18), 0);

	int nTotalHeight = 0;
	while (pScanLineTag->nTagSkip != nLength)
	{
		nTotalHeight += pTextBar->EnhScanText(lpText, nLength, pScanLineTag);

		//复制当前的行标记
		m_vecLineTag.push_back(new CLineTag(*pScanLineTag));
	}

	delete pScanLineTag;
	pScanLineTag = NULL;

	delete pTextBar;
	pTextBar = NULL;
}
