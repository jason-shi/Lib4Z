#include "StdAfx.h"
#include "PreClick.h"

CLDblClick			 g_tLDblClick;
CRDblClick			 g_tRDblClick;

void BlockLDblClick()
{
	UINT st = GetTickCount();
	MSG msg;
	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_LBUTTONUP)
			{
				//�洢���ݲ�ִ��
				g_tLDblClick.m_nRelFlags = msg.wParam;
				g_tLDblClick.m_pRelPoint = msg.lParam;
				g_tLDblClick.m_nButtonUp = 1;
				continue;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_LBUTTONDBLCLK)
			{
				//����DOWN��UP��Ϣ
				g_tLDblClick.m_nButtonUp = 2;
				return;
			}
		}
		UINT et = GetTickCount();
		if (et - st > 100)
			break;
	}
	//��ʱ����
	if (g_tLDblClick.m_nButtonUp == 1)
	{
		//��ԭһ�����Ƴ���WM_LBUTTONUP��Ϣ
		g_pZ2coo->SendMessage(WM_LBUTTONUP, g_tLDblClick.m_nRelFlags, g_tLDblClick.m_pRelPoint);
	}
}

void BlockRDblClick()
{
	UINT st = GetTickCount();
	MSG msg;
	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_RBUTTONUP)
			{
				//�洢���ݲ�ִ��
				g_tRDblClick.m_nRelFlags = msg.wParam;
				g_tRDblClick.m_pRelPoint = msg.lParam;
				g_tRDblClick.m_nButtonUp = 1;
				continue;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_RBUTTONDBLCLK)
			{
				//����DOWN��UP��Ϣ
				g_tRDblClick.m_nButtonUp = 2;
				return;
			}
		}
		UINT et = GetTickCount();
		if (et - st > 100)
			break;
	}
	//��ʱ����
	if (g_tRDblClick.m_nButtonUp == 1)
	{
		//��ԭһ�����Ƴ���WM_RBUTTONUP��Ϣ
		g_pZ2coo->SendMessage(WM_RBUTTONUP, g_tRDblClick.m_nRelFlags, g_tRDblClick.m_pRelPoint);
	}
}

void BlockKeepLDown(CObject *pActived, UINT nFlags, CPoint point)
{
	//��������һ�ε�������ֹ���͸�
	pActived->OnLClick(nFlags, point);

	//Aʱ������Ϊ������֮��ÿ��Bʱ���ⷢһ�ε���
	bool bKeepLDown = false;

	UINT st = GetTickCount();
	MSG msg;
	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_LBUTTONUP)
				break;
			if (msg.message == WM_MOUSEMOVE)
				point = msg.lParam;
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		UINT et = GetTickCount();
		if (et - st > 300)
			bKeepLDown = true;

		if (bKeepLDown)
		{
			if (et - st > 50)
			{
				st = et;

				//�������������ͣ�٣�����ƻ�����������
				CObject *pObject = g_pZ2coo->FindObject(point);
				if (pObject == pActived)
					pActived->OnLClick(nFlags, point);
			}
		}
	}
}

void BlockKeepRDown(CObject *pActived, UINT nFlags, CPoint point)
{
	//��������һ���ҵ�������ֹ���͸�
	pActived->OnRClick(nFlags, point);

	//Aʱ������Ϊ�ҵ�����֮��ÿ��Bʱ���ⷢһ���ҵ���
	bool bKeepRDown = false;

	UINT st = GetTickCount();
	MSG msg;
	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_RBUTTONUP)
				break;
			if (msg.message == WM_MOUSEMOVE)
				point = msg.lParam;
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		UINT et = GetTickCount();
		if (et - st > 300)
			bKeepRDown = true;

		if (bKeepRDown)
		{
			if (et - st > 50)
			{
				st = et;

				//�������������ͣ�٣�����ƻ�����������
				CObject *pObject = g_pZ2coo->FindObject(point);
				if (pObject == pActived)
					pActived->OnRClick(nFlags, point);
			}
		}
	}
}
