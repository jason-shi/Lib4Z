#include "StdAfx.h"
#include "DigitPanel.h"

CDigitPanel::CDigitPanel(CImage *pDigitImage, CRect xSrcRect, CSize xDigitSize, int nMaxDigit, char chFiller, bool bWithSign, int nSignGap)
				: CPanel(NULL, CPoint(0, 0), CSize(0, 0), 0)
{
	m_pDigitImage = pDigitImage;
	m_xSrcRect = xSrcRect;
	m_xDigitSize = xDigitSize;
	m_nMaxDigit = nMaxDigit;
	m_chFiller = chFiller;
	m_bWithSign = bWithSign;
	m_nSignGap = nSignGap;

	m_nNumber = 0;
}
 
CDigitPanel::~CDigitPanel()
{
	m_pDigitImage = NULL;
}

bool CDigitPanel::Initial()
{
	if (!CPanel::Initial())
		return false;

	//���÷���
	SpriteSign.Set(m_pDigitImage, CPoint(0, 0), CPoint(10 * m_xDigitSize.cx, m_xSrcRect.top), m_xDigitSize, 0);
	AddObject(&SpriteSign);

	//���÷���������
	for (int i = 0; i < 10; i++)
	{
		SpriteDigit[i].Set(m_pDigitImage, CPoint(0, 0), CPoint(0, m_xSrcRect.top), m_xDigitSize, 0);
		AddObject(&SpriteDigit[i]);
	}

	//��ʼ��ʱ����ʾ����
	ClearDigit();
	return true;
}

void CDigitPanel::Set(CImage *pDigitImage, CRect xSrcRect, CSize xDigitSize, int nMaxDigit, char chFiller, bool bWithSign, int nSignGap)
{
	m_pDigitImage = pDigitImage;
	m_xSrcRect = xSrcRect;
	m_xDigitSize = xDigitSize;
	m_nMaxDigit = nMaxDigit;
	m_chFiller = chFiller;
	m_bWithSign = bWithSign;
	m_nSignGap = nSignGap;
}

int CDigitPanel::GetTotalDigit() const
{
	int nTotalDigit = 0;

	int nNumber = abs(m_nNumber);
	while (nNumber != 0)
	{
		nNumber /= 10;
		nTotalDigit++;
	}
	return nTotalDigit == 0 ? 1 : nTotalDigit;
}

void CDigitPanel::ClearDigit()
{
	if (SpriteSign.IsShow())
		SpriteSign.Show(false);

	for (int i = 0; i < 10; i++)
	{
		if (SpriteDigit[i].IsShow())
			SpriteDigit[i].Show(false);
	}
}

void CDigitPanel::ShowDigit(int nNumber)
{
	m_nNumber = nNumber;

	ClearDigit();
	m_bWithSign ? ShowWithSign() : ShowWithoutSign();
}

void CDigitPanel::ShowWithSign()
{
	//���������Ծ��в�ͬ������
	int nSrcPosY = m_xSrcRect.top;
	if (m_nNumber < 0 && m_xSrcRect.Height() == 2 * m_xDigitSize.cy)
		nSrcPosY += m_xDigitSize.cy;

	//��ʾ����
	SpriteSign.SetSrcPos(10 * m_xDigitSize.cx, nSrcPosY);
	SpriteSign.SetDrawPos(0, 0);
	SpriteSign.Show();

	//������λ��
	int nTotalDigit = GetTotalDigit();
	if (nTotalDigit > m_nMaxDigit)
		return;

	//�������
	int nFillerWidth = 0;
	if (m_chFiller == '0' || m_chFiller == ' ')
	{
		nFillerWidth = (m_nMaxDigit - nTotalDigit) * m_xDigitSize.cx;
		if (m_chFiller == '0')
		{
			for (int i = 0; i < m_nMaxDigit - nTotalDigit; i++)
			{
				SpriteDigit[m_nMaxDigit - i - 1].SetSrcPos(0, nSrcPosY);
				SpriteDigit[m_nMaxDigit - i - 1].SetDrawPos((i + 1) * m_xDigitSize.cx + m_nSignGap, 0);
				SpriteDigit[m_nMaxDigit - i - 1].Show();
			}
		}
	}

	//�����������Ŀ��
	SetSize((nTotalDigit + 1) * m_xDigitSize.cx + m_nSignGap + nFillerWidth, m_xDigitSize.cy);

	//�ɵ͵�����ʾÿһλ����
	int nNumber = abs(m_nNumber);
	for (int i = 0; i < nTotalDigit; i++)
	{
		int n = nNumber % 10;
		nNumber /= 10;

		SpriteDigit[i].SetSrcPos(n * m_xDigitSize.cx, nSrcPosY);
		SpriteDigit[i].SetDrawPos((nTotalDigit - i) * m_xDigitSize.cx + m_nSignGap + nFillerWidth, 0);
		SpriteDigit[i].Show();
	}
}

void CDigitPanel::ShowWithoutSign()
{
	//���������Ծ��в�ͬ������
	int nSrcPosY = m_xSrcRect.top;
	if (m_nNumber < 0 && m_xSrcRect.Height() == 2 * m_xDigitSize.cy)
		nSrcPosY += m_xDigitSize.cy;

	//������λ��
	int nTotalDigit = GetTotalDigit();
	if (nTotalDigit > m_nMaxDigit)
		return;

	//�������
	int nFillerWidth = 0;
	if (m_chFiller == '0' || m_chFiller == ' ')
	{
		nFillerWidth = (m_nMaxDigit - nTotalDigit) * m_xDigitSize.cx;
		if (m_chFiller == '0')
		{
			for (int i = 0; i < m_nMaxDigit - nTotalDigit; i++)
			{
				SpriteDigit[m_nMaxDigit - i - 1].SetSrcPos(0, nSrcPosY);
				SpriteDigit[m_nMaxDigit - i - 1].SetDrawPos(i * m_xDigitSize.cx + m_nSignGap, 0);
				SpriteDigit[m_nMaxDigit - i - 1].Show();
			}
		}
	}

	//�����������Ŀ��
	SetSize(nTotalDigit * m_xDigitSize.cx + nFillerWidth, m_xDigitSize.cy);

	//�ɵ͵�����ʾÿһλ����
	int nNumber = abs(m_nNumber);
	for (int i = 0; i < nTotalDigit; i++)
	{
		int n = nNumber % 10;
		nNumber /= 10;

		SpriteDigit[i].SetSrcPos(n * m_xDigitSize.cx, nSrcPosY);
		SpriteDigit[i].SetDrawPos((nTotalDigit - i - 1) * m_xDigitSize.cx + nFillerWidth, 0);
		SpriteDigit[i].Show();
	}
}
