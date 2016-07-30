#include "StdAfx.h"
#include "PreClick.h"

enum {
	LS_NONE,
	LS_DRAGING,
	LS_DRAWING,
};

CZ2coo::CZ2coo()
{
	m_pActived = NULL;
	m_pContainer = NULL;
}

CZ2coo::~CZ2coo()
{
}

bool CZ2coo::Initial()
{
	m_pContainer = new CContainer();
	if (!m_pContainer || !m_pContainer->Initial())
		return false;

	return true;
}

void CZ2coo::Release()
{
	delete m_pContainer;
	m_pContainer = NULL;
}

bool CZ2coo::_PtInPressed(UINT nFlags, CPoint point)
{
	CObject *pPressed = m_pContainer->FindObject(m_xLDown);
	CObject *pObject = m_pContainer->FindObject(point);

	return pPressed && pPressed == pObject;
}

void CZ2coo::_ResetFocus(UINT nFlags, CPoint point)
{
	CObject *pObject = m_pContainer->FindObject(point);
	//失去焦点
	if (m_pActived && m_pActived != pObject)
	{
		m_pActived->OnLoseFocus();
		m_pActived = NULL;
	}
	//重获焦点
	if (pObject && m_pActived == NULL)
	{
		if (pObject->OnMouseEnter(nFlags, point))
			m_pActived = pObject;
	}
}

void CZ2coo::OnLButtonDown(UINT nFlags, CPoint point)
{
	CObject *pObject = m_pContainer->FindObject(point);
	if (pObject)
	{
		m_pActived = pObject;

		//处理LButtonDown事件
		m_pActived->OnLButtonDown(nFlags, point);

		m_xLDown = point;
		g_tLDblClick.m_nButtonUp = 0;

		//阻塞线程，双击检测
		if (m_pActived->IsEnableAction(MA_LDBLCLK))
			BlockLDblClick();
		//阻塞线程，连击检测
		if (m_pActived->IsEnableAction(MA_KEEPLDOWN))
			BlockKeepLDown(m_pActived, nFlags, point);
	}
}

void CZ2coo::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pActived)
	{
		switch (m_nLStatus)
		{
		case LS_NONE:
			_OnLButtonUp(nFlags, point);
			break;
		case LS_DRAGING:
			OnMouseDrop(nFlags, point);
			break;
		case LS_DRAWING:
			OnDrawOver(nFlags, point);
			break;
		}
	}
	_ResetFocus(nFlags, point);
}

void CZ2coo::_OnLButtonUp(UINT nFlags, CPoint point)
{
	//处理LButtonUp事件
	m_pActived->OnLButtonUp(nFlags, point);

	if (_PtInPressed(nFlags, point))
	{
		if (g_tLDblClick.m_nButtonUp == 2)
		{
			m_pActived->OnLDblClk(nFlags, point);
		}
		else
		{
			//0:未开双击检测或者双击检测超时未单击 1:双击检测超时已单击
			m_pActived->OnLClick(nFlags, point);
		}
	}
}

void CZ2coo::OnMouseMove(UINT nFlags, const CPoint& point)
{
	//处理MouseMove事件
	if (m_pActived == NULL)
		return _OnMouseMove(nFlags, point);

	switch (m_nLStatus)
	{
	case LS_NONE:
		//按照Draw, Drag, Move的优先级处理
		if (m_pActived->CheckDrawMode(nFlags) && CheckDrawThrs(point))
		{
			OnDrawRect(nFlags, point);
			m_nLStatus = LS_DRAWING;
			return;
		}
		if (m_pActived->CheckDragMode(nFlags) && CheckDragThrs(point))
		{
			OnMousePick(nFlags, point);
			m_nLStatus = LS_DRAGING;
			return;
		}
		_OnMouseMove(nFlags, point);
		break;
	case LS_DRAGING:
		if (m_pActived->CheckDragMode(nFlags))
			OnMouseDrag(nFlags, point);
		else
		{
			//不满足拖动模式时离开拖动状态（与拖动门限无关）
			OnMouseDrop(nFlags, point);
		}
		break;
	case LS_DRAWING:
		if (m_pActived->CheckDrawMode(nFlags))
			OnDrawRect(nFlags, point);
		else
		{
			//不满足框选模式时离开框选状态（与框选门限无关）
			OnDrawOver(nFlags, point);
		}
		break;
	}
}

void CZ2coo::_OnMouseMove(UINT nFlags, const CPoint& point)
{
	CObject *pObject = m_pContainer->FindObject(point);
	if (m_pActived && m_pActived != pObject)
	{
		//失去焦点
		if (m_pActived->OnMouseLeave(nFlags, point))
			m_pActived = NULL;
	}
	if (pObject && m_pActived == NULL)
	{
		//获得焦点
		if (pObject->OnMouseEnter(nFlags, point))
			m_pActived = pObject;
	}
	//在对象上移动
	if (pObject)
		pObject->OnMouseMove(nFlags, point);
}

void CZ2coo::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	CObject *pObject = m_pContainer->FindWheel(point);
	if (pObject)
	{
		m_pActived = pObject;
		m_pActived->OnMouseWheel(nFlags, zDelta, point);
	}
}

void CZ2coo::OnRButtonDown(UINT nFlags, CPoint point)
{
	CObject *pObject = m_pContainer->FindObject(point);
	if (pObject)
	{
		m_pActived = pObject;

		//处理LButtonDown事件
		m_pActived->OnRButtonDown(nFlags, point);

		m_xRDown = point;
		g_tRDblClick.m_nButtonUp = 0;

		//阻塞线程，双击检测
		if (m_pActived->IsEnableAction(MA_RDBLCLK))
			BlockRDblClick();
		//阻塞线程，连击检测
		if (m_pActived->IsEnableAction(MA_KEEPRDOWN))
			BlockKeepRDown(m_pActived, nFlags, point);
	}
}

void CZ2coo::OnRButtonUp(UINT nFlags, CPoint point)
{
	CObject *pObject = m_pContainer->FindObject(point);
	if (pObject)
	{
		pObject->OnRButtonUp(nFlags, point);
		if (m_pActived == pObject)
			m_pActived->OnRClick(nFlags, point);
	}
	_ResetFocus(nFlags, point);
}

void CZ2coo::OnMButtonDown(UINT nFlags, CPoint point)
{
	CObject *pObject = m_pContainer->FindObject(point);
	if (pObject)
	{
		m_pActived = pObject;
		m_pActived->OnMButtonDown(nFlags, point);
	}
}

void CZ2coo::OnMButtonUp(UINT nFlags, CPoint point)
{
	CObject *pObject = m_pContainer->FindObject(point);
	if (pObject)
	{
		pObject->OnMButtonUp(nFlags, point);
		if (m_pActived == pObject)
			m_pActived->OnMClick(nFlags, point);
	}
	_ResetFocus(nFlags, point);
}

bool CZ2coo::CheckDrawThrs(CPoint point)
{
	int nDrawThrs = m_pActived->GetDrawThrs();

	return abs(point.x - m_xLDown.x) >= nDrawThrs
		|| abs(point.y - m_xLDown.y) >= nDrawThrs;
}

void CZ2coo::OnDrawRect(UINT nFlags, const CPoint& point)
{
	CRect rect;
	rect.left   = min(m_xLDown.x, point.x);
	rect.right  = max(m_xLDown.x, point.x) + 1;
	rect.top    = min(m_xLDown.y, point.y);
	rect.bottom = max(m_xLDown.y, point.y) + 1;
	//限制在窗口内
	m_pActived->OnDrawRect(nFlags, rect & ClientRect);
}

void CZ2coo::OnDrawOver(UINT nFlags, const CPoint& point)
{
	CRect rect;
	rect.left   = min(m_xLDown.x, point.x);
	rect.right  = max(m_xLDown.x, point.x) + 1;
	rect.top    = min(m_xLDown.y, point.y);
	rect.bottom = max(m_xLDown.y, point.y) + 1;
	//限制在窗口内
	m_pActived->OnDrawOver(nFlags, rect & ClientRect);

	m_pActived = NULL;
	m_nLStatus = LS_NONE;
}

bool CZ2coo::CheckDragThrs(CPoint point)
{
	int nDragThrs = m_pActived->GetDragThrs();

	return abs(point.x - m_xLDown.x) >= nDragThrs
		|| abs(point.y - m_xLDown.y) >= nDragThrs;
}

void CZ2coo::OnMousePick(UINT nFlags, const CPoint& point)
{
	m_pActived->OnMousePick(nFlags, point);
}

void CZ2coo::OnMouseDrag(UINT nFlags, const CPoint& point)
{
	if (m_pActived->IsInteract())
	{
		CObject *pInteract = m_pContainer->FindExcept(point, m_pActived);
		if (pInteract)
			pInteract->OnDragInteract(m_pActived);
	}
	m_pActived->OnMouseDrag(nFlags, point);
}

void CZ2coo::OnMouseDrop(UINT nFlags, const CPoint& point)
{
	if (m_pActived->IsInteract())
	{
		CObject *pInteract = m_pContainer->FindExcept(point, m_pActived);
		if (pInteract)
			pInteract->OnDropInteract(m_pActived);
	}
	m_pActived->OnMouseDrop(nFlags, point);

	m_pActived = NULL;
	m_nLStatus = LS_NONE;
}

void CZ2coo::TerminateAction(UINT nFlags, const CPoint& point)
{
	switch (m_nLStatus)
	{
	case LS_DRAGING:
		OnMouseDrop(nFlags, point);
		break;
	case LS_DRAWING:
		OnDrawOver(nFlags, point);
		break;
	default:
		return;
	}

	m_pActived = NULL;
	m_nLStatus = LS_NONE;
}
