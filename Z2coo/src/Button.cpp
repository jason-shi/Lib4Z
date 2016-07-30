#include "StdAfx.h"

CButton::CButton() : CSprite()
{
}

CButton::CButton(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth)
			: CSprite(pImage, xDrawRect.TopLeft(), xSrcRect.TopLeft(), xDrawRect.Size(), nDepth)
{
	m_xSrcRect = xSrcRect;
	SetMapStyle();
	m_nStatus = BTN_NORMAL;
}

CButton::~CButton()
{
}

//将参数设定在构造好的对象上
void CButton::Set(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth)
{
	m_pImage = pImage;
	m_xDrawPos = xDrawRect.TopLeft();
	m_xSize = xDrawRect.Size();
	m_xSrcPos = xSrcRect.TopLeft();
	m_xSrcRect = xSrcRect;
	SetMapStyle();
	m_nDepth = nDepth;
	m_nStatus = BTN_NORMAL;
}

void CButton::SetSrcPos(int x, int y)
{
	m_xSrcRect.OffsetRect(x - m_xSrcRect.left, y - m_xSrcRect.top);
	SetMapSrcPos();
	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

void CButton::SetMapStyle()
{
	int W = m_xSrcRect.Width();
	int H = m_xSrcRect.Height();
	//支持四种映射类型
	if (W == 4 * m_xSize.cx && H == m_xSize.cy) //XXXX
		m_nMapStyle = 1;
	else if (W == 2 * m_xSize.cx && H == 2 * m_xSize.cy) //XX, XX
		m_nMapStyle = 2;
	else if (W == m_xSize.cx && H == 4 * m_xSize.cy) //X, X, X, X
		m_nMapStyle = 3;
	else //X 或其他
		m_nMapStyle = 0;
}

void CButton::SetMapSrcPos()
{
	static int HorzT[4 * 4] = {
		0, 0, 0, 0, //X
		0, 1, 2, 3, //XXXX
		0, 1, 0, 1, //XX, XX
		0, 0, 0, 0, //X, X, X, X
	};
	static int VertT[4 * 4] = {
		0, 0, 0, 0, //X
		0, 0, 0, 0, //XXXX
		0, 0, 1, 1, //XX, XX
		0, 1, 2, 3, //X, X, X, X
	};
	int nIndex = (m_nMapStyle << 2) + m_nStatus;
	m_xSrcPos.x = m_xSrcRect.left + HorzT[nIndex] * m_xSize.cx;
	m_xSrcPos.y = m_xSrcRect.top + VertT[nIndex] * m_xSize.cy;
}

void CButton::Disable(bool bDisable)
{
	m_nStatus = bDisable ? BTN_DISABLE : BTN_NORMAL;
	SetMapSrcPos();

	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

CObject *CButton::FindObject(const CPoint& point)
{
	if (m_bCease || PtInImage(point))
		return this;

	return NULL;
}

CObject *CButton::FindExcept(const CPoint& point, CObject *pExcept)
{
	//本身是排除对象
	if (pExcept == this)
		return NULL;

	if (m_bCease || PtInImage(point))
		return this;

	return NULL;
}

void CButton::OnLoseFocus()
{
	if (m_nStatus == BTN_DISABLE)
		return;

	m_nStatus = BTN_NORMAL;
	SetMapSrcPos();

	//发送重绘消息
	g_pZ2coo->Redraw(this);
}

void CButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nStatus == BTN_DISABLE)
		return;

	m_nStatus = BTN_PRESSED;
	SetMapSrcPos();

	//发送重绘消息
	g_pZ2coo->Redraw(this);

	CObject::OnLButtonDown(nFlags, point);
}

void CButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_nStatus == BTN_DISABLE)
		return;

	m_nStatus = BTN_FOCUSED;
	SetMapSrcPos();

	//发送重绘消息
	g_pZ2coo->Redraw(this);

	CObject::OnLButtonUp(nFlags, point);
}

bool CButton::OnMouseEnter(UINT nFlags, CPoint point)
{
	if (m_nStatus != BTN_NORMAL)
		return false;

	m_nStatus = BTN_FOCUSED;
	SetMapSrcPos();

	//发送重绘消息
	g_pZ2coo->Redraw(this);

	//基类处理
	CSprite::OnMouseEnter(nFlags, point);
	return true;
}

bool CButton::OnMouseLeave(UINT nFlags, CPoint point)
{
	if (m_nStatus != BTN_FOCUSED)
		return false;

	m_nStatus = BTN_NORMAL;
	SetMapSrcPos();

	//发送重绘消息
	g_pZ2coo->Redraw(this);

	//基类处理
	CSprite::OnMouseLeave(nFlags, point);
	return true;
}

void CButton::OnLClick(UINT nFlags, CPoint point)
{
	if (m_nStatus == BTN_DISABLE)
		return;

	return CObject::OnLClick(nFlags, point);
}

void CButton::OnRClick(UINT nFlags, CPoint point)
{
	if (m_nStatus == BTN_DISABLE)
		return;

	return CObject::OnRClick(nFlags, point);
}

void CButton::OnMClick(UINT nFlags, CPoint point)
{
	if (m_nStatus == BTN_DISABLE)
		return;

	return CObject::OnMClick(nFlags, point);
}
