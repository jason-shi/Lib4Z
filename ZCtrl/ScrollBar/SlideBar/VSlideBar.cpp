#include "StdAfx.h"
#include "VSlideBar.h"
#include "SlideBtn.h"

CVSlideBar::CVSlideBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize)
				: CSlideBar(pImage, xDrawPos, xSrcPos, xSize)
{
	m_pUCap = NULL;
	m_pUPad = NULL;
	m_pCore = NULL;
	m_pDPad = NULL;
	m_pDCap = NULL;
}

CVSlideBar::~CVSlideBar()
{
	delete m_pUCap;
	delete m_pUPad;
	delete m_pCore;
	delete m_pDPad;
	delete m_pDCap;
}

bool CVSlideBar::Initial()
{
	if (!CPanel::Initial())
		return false;

	m_pUCap = new CSlideBtn(&ImageCtrl, CRect(0, 0, 13, 7),  CRect(1, 45, 14, 52), 0);
	if (!m_pUCap || !m_pUCap->Initial())
		return false;
	m_pUCap->SetHome(this);
	AddObject(m_pUCap);

	m_pUPad = new CSlideBtn(&ImageCtrl, CRect(0, 7, 13, 8),  CRect(15, 0, 28, 1), 0);
	if (!m_pUPad || !m_pUPad->Initial())
		return false;
	m_pUPad->SetHome(this);
	AddObject(m_pUPad);

	m_pCore = new CSlideBtn(&ImageCtrl, CRect(0, 0, 0, 0),   CRect(0, 0, 0, 0), 0);
	if (!m_pCore || !m_pCore->Initial())
		return false;
	m_pCore->SetHome(this);
	AddObject(m_pCore);

	m_pDPad = new CSlideBtn(&ImageCtrl, CRect(0, 8, 13, 9),  CRect(15, 0, 28, 1), 0);
	if (!m_pDPad || !m_pDPad->Initial())
		return false;
	m_pDPad->SetHome(this);
	AddObject(m_pDPad);

	m_pDCap = new CSlideBtn(&ImageCtrl, CRect(0, 9, 13, 16), CRect(1, 53, 14, 60), 0);
	if (!m_pDCap || !m_pDCap->Initial())
		return false;
	m_pDCap->SetHome(this);
	AddObject(m_pDCap);

	return true;
}

void CVSlideBar::Set(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
{
	CSlideBar::Set(pImage, xDrawPos, xSrcPos, xSize, nDepth);

	m_pUCap->Set(&ImageCtrl, CRect(0, 0, 13, 7),  CRect(1, 45, 14, 52), 0);
	m_pUPad->Set(&ImageCtrl, CRect(0, 7, 13, 8),  CRect(15, 0, 28, 1), 0);
	m_pCore->Set(&ImageCtrl, CRect(0, 0, 0, 0),   CRect(0, 0, 0, 0), 0);
	m_pDPad->Set(&ImageCtrl, CRect(0, 8, 13, 9),  CRect(15, 0, 28, 1), 0);
	m_pDCap->Set(&ImageCtrl, CRect(0, 9, 13, 16), CRect(1, 53, 14, 60), 0);
}

void CVSlideBar::SetHeight(int nBarHeight)
{
	int nCapHeight = m_pUCap->GetSize().cy;
	int nCoreHeight = m_pCore->GetSize().cy;
	int nRsvdHeight = nCapHeight * 2 + nCoreHeight;

	//滑块高度调整
	if (nBarHeight < nRsvdHeight)
		nBarHeight = nRsvdHeight;

	int nPadHeight = (nBarHeight - nRsvdHeight) / 2;

	//设置顶帽
	int y = 0;
	m_pUCap->SetDrawPos(m_pUCap->GetDrawPos().x, y);
	m_pUCap->SetHeight(nCapHeight);

	//设置上填充
	y += nCapHeight;
	m_pUPad->SetDrawPos(m_pUPad->GetDrawPos().x, y);
	m_pUPad->SetHeight(nPadHeight);

	//设置触点
	y += nPadHeight;
	m_pCore->SetDrawPos(m_pCore->GetDrawPos().x, y);
	m_pCore->SetHeight(nCoreHeight);

	//非对称调整
	if ((nBarHeight - nRsvdHeight) & 1)
		nPadHeight += 1;

	//设置下填充
	y += nCoreHeight;
	m_pDPad->SetDrawPos(m_pDPad->GetDrawPos().x, y);
	m_pDPad->SetHeight(nPadHeight);

	//设置底帽
	y += nPadHeight;
	m_pDCap->SetDrawPos(m_pDCap->GetDrawPos().x, y);
	m_pDCap->SetHeight(nCapHeight);

	//调整自身高度
	m_xSize.cy = nBarHeight;
}
