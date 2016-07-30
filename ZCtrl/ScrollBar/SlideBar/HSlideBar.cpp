#include "StdAfx.h"
#include "HSlideBar.h"
#include "SlideBtn.h"

CHSlideBar::CHSlideBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize)
				: CSlideBar(pImage, xDrawPos, xSrcPos, xSize)
{
	m_pLCap = NULL;
	m_pLPad = NULL;
	m_pCore = NULL;
	m_pRPad = NULL;
	m_pRCap = NULL;
}

CHSlideBar::~CHSlideBar()
{
	delete m_pLCap;
	delete m_pLPad;
	delete m_pCore;
	delete m_pRPad;
	delete m_pRCap;
}

bool CHSlideBar::Initial()
{
	if (!CPanel::Initial())
		return false;

	m_pLCap = new CSlideBtn(&ImageCtrl, CRect(0, 0, 7, 13),  CRect(444, 14, 451, 27), 0);
	if (!m_pLCap || !m_pLCap->Initial())
		return false;
	m_pLCap->SetHome(this);
	AddObject(m_pLCap);

	m_pLPad = new CSlideBtn(&ImageCtrl, CRect(7, 0, 8, 13),  CRect(399, 0, 400, 13), 0);
	if (!m_pLPad || !m_pLPad->Initial())
		return false;
	m_pLPad->SetHome(this);
	AddObject(m_pLPad);

	m_pCore = new CSlideBtn(&ImageCtrl, CRect(0, 0, 0, 0),   CRect(0, 0, 0, 0), 0);
	if (!m_pCore || !m_pCore->Initial())
		return false;
	m_pCore->SetHome(this);
	AddObject(m_pCore);

	m_pRPad = new CSlideBtn(&ImageCtrl, CRect(8, 0, 9, 13),  CRect(399, 0, 400, 13), 0);
	if (!m_pRPad || !m_pRPad->Initial())
		return false;
	m_pRPad->SetHome(this);
	AddObject(m_pRPad);

	m_pRCap = new CSlideBtn(&ImageCtrl, CRect(9, 0, 16, 13), CRect(452, 14, 459, 27), 0);
	if (!m_pRCap || !m_pRCap->Initial())
		return false;
	m_pRCap->SetHome(this);
	AddObject(m_pRCap);

	return true;
}

void CHSlideBar::Set(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
{
	CSlideBar::Set(pImage, xDrawPos, xSrcPos, xSize, nDepth);

	m_pLCap->Set(&ImageCtrl, CRect(0, 0, 7, 13),  CRect(444, 14, 451, 27), 0);
	m_pLPad->Set(&ImageCtrl, CRect(7, 0, 8, 13),  CRect(399, 0, 400, 13), 0);
	m_pCore->Set(&ImageCtrl, CRect(0, 0, 0, 0),   CRect(0, 0, 0, 0), 0);
	m_pRPad->Set(&ImageCtrl, CRect(8, 0, 9, 13),  CRect(399, 0, 400, 13), 0);
	m_pRCap->Set(&ImageCtrl, CRect(9, 0, 16, 13), CRect(452, 14, 459, 27), 0);
}

void CHSlideBar::SetWidth(int nBarWidth)
{
	int nCapWidth = m_pLCap->GetSize().cx;
	int nCoreWidth = m_pCore->GetSize().cx;
	int nRsvdWidth = nCapWidth * 2 + nCoreWidth;

	//滑块宽度调整
	if (nBarWidth < nRsvdWidth)
		nBarWidth = nRsvdWidth;

	int nPadWidth = (nBarWidth - nRsvdWidth) / 2;

	//设置左边帽
	int x = 0;
	m_pLCap->SetDrawPos(x, m_pLCap->GetDrawPos().y);
	m_pLCap->SetWidth(nCapWidth);

	//设置左填充
	x += nCapWidth;
	m_pLPad->SetDrawPos(x, m_pLPad->GetDrawPos().y);
	m_pLPad->SetWidth(nPadWidth);

	//设置触点
	x += nPadWidth;
	m_pCore->SetDrawPos(x, m_pCore->GetDrawPos().y);
	m_pCore->SetWidth(nCoreWidth);

	//非对称调整
	if ((nBarWidth - nRsvdWidth) & 1)
		nPadWidth += 1;

	//设置右填充
	x += nCoreWidth;
	m_pRPad->SetDrawPos(x, m_pRPad->GetDrawPos().y);
	m_pRPad->SetWidth(nPadWidth);

	//设置右边帽
	x += nPadWidth;
	m_pRCap->SetDrawPos(x, m_pRCap->GetDrawPos().y);
	m_pRCap->SetWidth(nCapWidth);

	//调整自身宽度
	m_xSize.cx = nBarWidth;
}
