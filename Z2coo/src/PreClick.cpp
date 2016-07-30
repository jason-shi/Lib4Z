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
				//存储且暂不执行
				g_tLDblClick.m_nRelFlags = msg.wParam;
				g_tLDblClick.m_pRelPoint = msg.lParam;
				g_tLDblClick.m_nButtonUp = 1;
				continue;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_LBUTTONDBLCLK)
			{
				//丢弃DOWN和UP消息
				g_tLDblClick.m_nButtonUp = 2;
				return;
			}
		}
		UINT et = GetTickCount();
		if (et - st > 100)
			break;
	}
	//超时结束
	if (g_tLDblClick.m_nButtonUp == 1)
	{
		//还原一条被移除的WM_LBUTTONUP消息
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
				//存储且暂不执行
				g_tRDblClick.m_nRelFlags = msg.wParam;
				g_tRDblClick.m_pRelPoint = msg.lParam;
				g_tRDblClick.m_nButtonUp = 1;
				continue;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_RBUTTONDBLCLK)
			{
				//丢弃DOWN和UP消息
				g_tRDblClick.m_nButtonUp = 2;
				return;
			}
		}
		UINT et = GetTickCount();
		if (et - st > 100)
			break;
	}
	//超时结束
	if (g_tRDblClick.m_nButtonUp == 1)
	{
		//还原一条被移除的WM_RBUTTONUP消息
		g_pZ2coo->SendMessage(WM_RBUTTONUP, g_tRDblClick.m_nRelFlags, g_tRDblClick.m_pRelPoint);
	}
}

void BlockKeepLDown(CObject *pActived, UINT nFlags, CPoint point)
{
	//立即产生一次单击，防止阻滞感
	pActived->OnLClick(nFlags, point);

	//A时间以内为单击，之后每隔B时间拟发一次单击
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

				//鼠标移走则连击停顿，鼠标移回则连击继续
				CObject *pObject = g_pZ2coo->FindObject(point);
				if (pObject == pActived)
					pActived->OnLClick(nFlags, point);
			}
		}
	}
}

void BlockKeepRDown(CObject *pActived, UINT nFlags, CPoint point)
{
	//立即产生一次右单击，防止阻滞感
	pActived->OnRClick(nFlags, point);

	//A时间以内为右单击，之后每隔B时间拟发一次右单击
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

				//鼠标移走则连击停顿，鼠标移回则连击继续
				CObject *pObject = g_pZ2coo->FindObject(point);
				if (pObject == pActived)
					pActived->OnRClick(nFlags, point);
			}
		}
	}
}
