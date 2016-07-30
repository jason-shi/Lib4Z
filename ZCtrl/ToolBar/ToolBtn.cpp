#include "StdAfx.h"
#include "ToolBtn.h"

CToolBtn::CToolBtn(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth)
			: CButton(pImage, xDrawRect, xSrcRect, nDepth)
{
	m_bSelected = false;
	OnSwitchSelect = NULL;
}

CToolBtn::~CToolBtn()
{
}

void CToolBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nStatus == BTN_DISABLE)
		return;

	m_nStatus = BTN_PRESSED;
	SetMapSrcPos();
	//发送重绘消息
	g_pZ2coo->Redraw(this);

	CObject::OnLButtonDown(nFlags, point);
}

void CToolBtn::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_nStatus == BTN_DISABLE)
		return;

	if (m_nStatus == BTN_PRESSED)
	{
		if (OnSwitchSelect)
			OnSwitchSelect(this, nFlags, point);
		m_nStatus = BTN_FOCUSED;
	}
	SetMapSrcPos();

	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

bool CToolBtn::OnMouseEnter(UINT nFlags, CPoint point)
{
	if (m_nStatus == BTN_DISABLE)
		return false;

	m_nStatus = BTN_FOCUSED;
	SetMapSrcPos();

	//发送重绘消息
	g_pZ2coo->Redraw(this);

	//基类处理
	CSprite::OnMouseEnter(nFlags, point);
	return true;
}

bool CToolBtn::OnMouseLeave(UINT nFlags, CPoint point)
{
	if (m_nStatus == BTN_DISABLE)
		return false;

	m_nStatus = IsSelected() ? BTN_PRESSED : BTN_NORMAL;
	SetMapSrcPos();

	//发送重绘消息
	g_pZ2coo->Redraw(this);

	//基类处理
	CSprite::OnMouseLeave(nFlags, point);
	return true;
}

void CToolBtn::OnSelect()
{
	m_bSelected = true;

	m_nStatus = BTN_PRESSED;
	SetMapSrcPos();

	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

void CToolBtn::UnSelect()
{
	m_bSelected = false;

	m_nStatus = BTN_NORMAL;
	SetMapSrcPos();

	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

bool CToolBtn::ProcessLClick(CObject *pObject, UINT nFlags, CPoint point)
{
	return true;
}

bool CToolBtn::ProcessRClick(CObject *pObject, UINT nFlags, CPoint point)
{
	return true;
}

bool CToolBtn::ProcessDrawRect(CObject *pObject, UINT nFlags, const CRect& rect)
{
	return true;
}
