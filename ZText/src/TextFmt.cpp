#include "StdAfx.h"
#include "TextFmt.h"

CTextFmt::CTextFmt(int nCapacity) : m_nCapacity(nCapacity)
{
	m_wcsBuff = NULL;
	m_nLength = 0;
}

CTextFmt::~CTextFmt()
{
	delete m_wcsBuff;
	m_wcsBuff = NULL;
}

bool CTextFmt::Initial()
{
	m_wcsBuff = new TCHAR[m_nCapacity];
	if (!m_wcsBuff)
		return false;

	return true;
}

bool CTextFmt::Release()
{
	delete m_wcsBuff;
	m_wcsBuff = NULL;

	return true;
}

void CTextFmt::WriteText(TCHAR *pText, int L)
{
	_SetShort(TF_TEXT);
	_SetShort(L);
	_SetString(pText, L);
}

void CTextFmt::WriteFamily(TCHAR *pFamily, int L)
{
	_SetShort(TF_FAMILY);
	_SetShort(L);
	_SetString(pFamily, L);
}

void CTextFmt::WriteWidth(short shWidth)
{
	_SetShort(TF_WIDTH);
	_SetShort(2);
	_SetShort(shWidth);
}

void CTextFmt::WriteHeight(short shHeight)
{
	_SetShort(TF_HEIGHT);
	_SetShort(2);
	_SetShort(shHeight);
}

void CTextFmt::WriteTransparent(bool bTransparent)
{
	_SetShort(TF_TRANSPARENT);
	_SetShort(1);
	_SetBool(bTransparent);
}

void CTextFmt::WriteColor(COLORREF color)
{
	_SetShort(TF_COLOR);
	_SetShort(4);
	_SetColor(color);
}

void CTextFmt::WriteBkColor(COLORREF bkcolor)
{
	_SetShort(TF_BKCOLOR);
	_SetShort(4);
	_SetColor(bkcolor);
}

void CTextFmt::WriteStyle(char chStyle)
{
	_SetShort(TF_STYLE);
	_SetShort(1);
	_SetChar(chStyle);
}

void CTextFmt::WriteXSpace(short shXSpace)
{
	_SetShort(TF_X_SPACE);
	_SetShort(2);
	_SetShort(shXSpace);
}

void CTextFmt::WriteYSpace(short shYSpace)
{
	_SetShort(TF_Y_SPACE);
	_SetShort(2);
	_SetShort(shYSpace);
}

void CTextFmt::WriteReturn()
{
	_SetShort(TF_RETURN);
	_SetShort(0);
}

void CTextFmt::_SetBool(const bool value)
{
	m_wcsBuff[m_nLength++] = (byte)value;
}

void CTextFmt::_SetChar(const char value)
{
	m_wcsBuff[m_nLength++] = (byte)value;
}

void CTextFmt::_SetShort(const short value)
{
	m_wcsBuff[m_nLength++] = (byte)(value >> 8);
	m_wcsBuff[m_nLength++] = (byte)(value >> 0);
}

void CTextFmt::_SetColor(const COLORREF color)
{
	m_wcsBuff[m_nLength++] = (byte)(color >> 0);
	m_wcsBuff[m_nLength++] = (byte)(color >> 8);
	m_wcsBuff[m_nLength++] = (byte)(color >> 16);
	m_wcsBuff[m_nLength++] = (byte)(color >> 24);
}

void CTextFmt::_SetString(const TCHAR *pString, int L)
{
	wcscpy_s(m_wcsBuff + m_nLength, L + 1, pString);
	m_nLength += L;
}
