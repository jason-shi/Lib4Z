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
	//ʧȥ����
	if (m_pActived && m_pActived != pObject)
	{
		m_pActived->OnLoseFocus();
		m_pActived = NULL;
	}
	//�ػ񽹵�
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

		//����LButtonDown�¼�
		m_pActived->OnLButtonDown(nFlags, point);

		m_xLDown = point;
		g_tLDblClick.m_nButtonUp = 0;

		//�����̣߳�˫�����
		if (m_pActived->IsEnableAction(MA_LDBLCLK))
			BlockLDblClick();
		//�����̣߳��������
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
	//����LButtonUp�¼�
	m_pActived->OnLButtonUp(nFlags, point);

	if (_PtInPressed(nFlags, point))
	{
		if (g_tLDblClick.m_nButtonUp == 2)
		{
			m_pActived->OnLDblClk(nFlags, point);
		}
		else
		{
			//0:δ��˫��������˫����ⳬʱδ���� 1:˫����ⳬʱ�ѵ���
			m_pActived->OnLClick(nFlags, point);
		}
	}
}

void CZ2coo::OnMouseMove(UINT nFlags, const CPoint& point)
{
	//����MouseMove�¼�
	if (m_pActived == NULL)
		return _OnMouseMove(nFlags, point);

	switch (m_nLStatus)
	{
	case LS_NONE:
		//����Draw, Drag, Move�����ȼ�����
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
			//�������϶�ģʽʱ�뿪�϶�״̬�����϶������޹أ�
			OnMouseDrop(nFlags, point);
		}
		break;
	case LS_DRAWING:
		if (m_pActived->CheckDrawMode(nFlags))
			OnDrawRect(nFlags, point);
		else
		{
			//�������ѡģʽʱ�뿪��ѡ״̬�����ѡ�����޹أ�
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
		//ʧȥ����
		if (m_pActived->OnMouseLeave(nFlags, point))
			m_pActived = NULL;
	}
	if (pObject && m_pActived == NULL)
	{
		//��ý���
		if (pObject->OnMouseEnter(nFlags, point))
			m_pActived = pObject;
	}
	//�ڶ������ƶ�
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

		//����LButtonDown�¼�
		m_pActived->OnRButtonDown(nFlags, point);

		m_xRDown = point;
		g_tRDblClick.m_nButtonUp = 0;

		//�����̣߳�˫�����
		if (m_pActived->IsEnableAction(MA_RDBLCLK))
			BlockRDblClick();
		//�����̣߳��������
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
	//�����ڴ�����
	m_pActived->OnDrawRect(nFlags, rect & ClientRect);
}

void CZ2coo::OnDrawOver(UINT nFlags, const CPoint& point)
{
	CRect rect;
	rect.left   = min(m_xLDown.x, point.x);
	rect.right  = max(m_xLDown.x, point.x) + 1;
	rect.top    = min(m_xLDown.y, point.y);
	rect.bottom = max(m_xLDown.y, point.y) + 1;
	//�����ڴ�����
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
