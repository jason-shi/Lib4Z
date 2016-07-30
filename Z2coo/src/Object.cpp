#include "StdAfx.h"

CPoint CObject::DragOffset = CPoint(0, 0);

CObject::CObject() : CWater()
{
	m_nObjId = 0;
	m_xOffset.x = 0;
	m_xOffset.y = 0;
	m_xDrawPos.x = 0;
	m_xDrawPos.y = 0;
	m_xSize.cx = 0;
	m_xSize.cy = 0;
	m_nDepth = 0;
	m_bCease = false;
	m_bShow = true;
	m_bExist = false;
	m_bInteract = false;
	m_byDragMode = 0xFF;
	m_byDrawMode = 0xFF;
	m_nMouseAction = 0;
	m_nEventTrack = 0;
	m_pNext = NULL;
	m_pHome = NULL;
	m_pParent = NULL;
	memset(OnMouseProc, NULL, sizeof(OnMouseProc));
}

CObject::CObject(CPoint xDrawPos, CSize xSize, int nDepth)
{
	m_nObjId = 0;
	m_xOffset.x = 0;
	m_xOffset.y = 0;
	m_xDrawPos = xDrawPos;
	m_xSize = xSize;
	m_nDepth = nDepth;
	m_bCease = false;
	m_bShow = true;
	m_bExist = false;
	m_bInteract = false;
	m_byDragMode = 0xFF;
	m_byDrawMode = 0xFF;
	m_nMouseAction = 0;
	m_nEventTrack = 0;
	m_pNext = NULL;
	m_pHome = NULL;
	m_pParent = NULL;
	memset(OnMouseProc, NULL, sizeof(OnMouseProc));
}

CObject::~CObject()
{
	if (g_pZ2coo->IsActived(this))
	{
		//去激活当前对象
		g_pZ2coo->SetActived(NULL);
	}
}

void CObject::SetDrawPos(int x, int y)
{
	CRect rect;
	GetRect(&rect);
	//重置绘制点
	m_xDrawPos.x = x;
	m_xDrawPos.y = y;
	g_pZ2coo->Redraw(this, rect);
}

void CObject::Offset(int x, int y)
{
	CRect rect;
	GetRect(&rect);
	//重置绘制点
	m_xDrawPos.x += x;
	m_xDrawPos.y += y;
	g_pZ2coo->Redraw(this, rect);
}

void CObject::AdjustAnchor(CPoint point, const CRect& rect)
{
	if (point.x + m_xSize.cx - 1 > rect.right)
		point.x -= m_xSize.cx;
	if (point.y + m_xSize.cy - 1 > rect.bottom)
		point.y -= m_xSize.cy;

	SetDrawPos(point);
}

void CObject::AdjustCenter(const CRect& rect, byte byFlag)
{
	int x = m_xDrawPos.x;
	int y = m_xDrawPos.y;

	if (byFlag & ADC_X_DIRECTION)
		x = (rect.Width() - m_xSize.cx) >> 1;
	if (byFlag & ADC_Y_DIRECTION)
		y = (rect.Height() - m_xSize.cy) >> 1;

	SetDrawPos(x, y);
}

void CObject::SetSize(int cx, int cy)
{
	CRect rect;
	GetRect(&rect);
	//重置大小
	m_xSize.cx = cx;
	m_xSize.cy = cy;
	g_pZ2coo->Redraw(this, rect);
}

void CObject::Show(bool bShow)
{
	m_bShow = bShow;
	g_pZ2coo->Redraw(this);
}

void CObject::RevDraw(CImage *pImage)
{
	if (m_pNext)
		m_pNext->RevDraw(pImage);
	if (m_bShow)
		Draw(pImage);
}

void CObject::RevDraw(CImage *pImage, const CRect& rect)
{
	if (m_pNext)
		m_pNext->RevDraw(pImage, rect);
	if (m_bShow)
		Draw(pImage, rect);
}

void CObject::Draw(CImage *pImage)
{
}

void CObject::Draw(CImage *pImage, const CRect& rect)
{
}

CObject *CObject::FindObject(const CPoint& point)
{
	return NULL;
}

CObject *CObject::FindWheel(const CPoint& point)
{
	return NULL;
}

CObject *CObject::FindExcept(const CPoint& point, CObject *pExcept)
{
	return NULL;
}

void CObject::OnLoseFocus()
{
}

void CObject::OnLButtonDown(UINT nFlags, CPoint point)
{
	DragOffset = point + -(m_xOffset + m_xDrawPos);
}

void CObject::OnLButtonUp(UINT nFlags, CPoint point)
{
}

void CObject::OnLClick(UINT nFlags, CPoint point)
{
	if (OnMouseProc[MP_LCLICK])
		OnMouseProc[MP_LCLICK](this, nFlags, point);
}

void CObject::OnRClick(UINT nFlags, CPoint point)
{
	if (OnMouseProc[MP_RCLICK])
		OnMouseProc[MP_RCLICK](this, nFlags, point);
}

void CObject::OnMClick(UINT nFlags, CPoint point)
{
	if (OnMouseProc[MP_MCLICK])
		OnMouseProc[MP_MCLICK](this, nFlags, point);
}

void CObject::OnLDblClk(UINT nFlags, CPoint point)
{
}

void CObject::OnRDblClk(UINT nFlags, CPoint point)
{
}

void CObject::OnMDblClk(UINT nFlags, CPoint point)
{
}

void CObject::OnMouseMove(UINT nFlags, CPoint point)
{
}

void CObject::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
}

bool CObject::OnMouseEnter(UINT nFlags, CPoint point)
{
	if (OnMouseProc[MP_MOUSEENTER])
		OnMouseProc[MP_MOUSEENTER](this, nFlags, point);
	return false;
}

bool CObject::OnMouseLeave(UINT nFlags, CPoint point)
{
	if (OnMouseProc[MP_MOUSELEAVE])
		OnMouseProc[MP_MOUSELEAVE](this, nFlags, point);
	return false;
}

void CObject::OnDragInteract(CObject *pObject)
{
}

void CObject::OnDropInteract(CObject *pObject)
{
}

void CObject::OnRButtonDown(UINT nFlags, CPoint point)
{
}

void CObject::OnRButtonUp(UINT nFlags, CPoint point)
{
}

void CObject::OnMButtonDown(UINT nFlags, CPoint point)
{
}

void CObject::OnMButtonUp(UINT nFlags, CPoint point)
{
}

int CObject::GetDrawThrs()
{
	return 0;
}

void CObject::OnDrawRect(UINT nFlags, const CRect& rect)
{
}

void CObject::OnDrawOver(UINT nFlags, const CRect& rect)
{
}

int CObject::GetDragThrs() const
{
	return 0;
}

void CObject::OnMousePick(UINT nFlags, CPoint point)
{
	if (OnMouseProc[MP_MOUSEPICK])
		OnMouseProc[MP_MOUSEPICK](this, nFlags, point);
}

void CObject::OnMouseDrag(UINT nFlags, CPoint point)
{
	if (OnMouseProc[MP_MOUSEDRAG])
		OnMouseProc[MP_MOUSEDRAG](this, nFlags, point);
}

void CObject::OnMouseDrop(UINT nFlags, CPoint point)
{
	if (OnMouseProc[MP_MOUSEDROP])
		OnMouseProc[MP_MOUSEDROP](this, nFlags, point);
}

void CObject::OnTrackLeave(UINT nFlags, CPoint point)
{
}

void CObject::OnTrackHover(UINT nFlags, CPoint point)
{
}
