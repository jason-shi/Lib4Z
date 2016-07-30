#include "StdAfx.h"
#include "ZTimer.h"
#include <boost\bind.hpp>

static HANDLE		 g_hThreadTimer = NULL;
static bool			 g_bFlagUpdate;
static bool			 g_bFlagDestroy;

UINT WINAPI ThreadTimer(LPVOID lpParameter)
{
	int nStartTick = GetTickCount();
	while (!g_bFlagDestroy)
	{
		int nEndTick = GetTickCount();

		//定时器循环在主线程
		g_pZTimer->SendMessage(WM_TIMER_UPDATE, nEndTick - nStartTick);

		nStartTick = nEndTick;
		::Sleep(10);
	}
	g_bFlagUpdate = false;
	return 0;
}

bool InitThreadTimer()
{
	if (!g_hThreadTimer)
	{
		g_hThreadTimer = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadTimer, NULL, 0, NULL);
		if (g_hThreadTimer)
		{
			CloseHandle(g_hThreadTimer);
			g_hThreadTimer = NULL;
			return true;
		}
	}
	return false;
}

CZTimer::CZTimer()
{
}

CZTimer::~CZTimer()
{
}

bool CZTimer::Initial()
{
	//启动线程
	g_bFlagUpdate = true;
	g_bFlagDestroy = false;

	return InitThreadTimer();
}

void CZTimer::Release()
{
	//循环等待直到线程结束
	if (g_bFlagUpdate)
	{
		g_bFlagDestroy = true;

		//在定时器线程释放之前，还要继续处理消息
		MSG msg;
		while (g_bFlagUpdate)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message <= WM_USER)
					continue;
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}
}

void CZTimer::AddTimer(int type, int timeout, FN_TimerProc TimerProc, void *lpParam)
{
	mutex.Lock();
	CTickTimer tTickTimer(type, timeout, TimerProc, lpParam, false);
	m_lstTickTimer.push_back(tTickTimer);
	mutex.Unlock();
}

void CZTimer::DelTimer(int type)
{
	mutex.Lock();
	CTickTimer tTickTimer(type, 0, NULL, 0, true);
	m_lstTickTimer.push_back(tTickTimer);
	mutex.Unlock();
}

void CZTimer::ClearTimer()
{
	mutex.Lock();
	TimerList::iterator it = m_lstTickTimer.begin();
	for (; it != m_lstTickTimer.end(); it++)
	{
		it->m_bToRemove = true;
		m_lstTickTimer.push_back(*it);
	}
	mutex.Unlock();
}

void CZTimer::SetPause(bool bPause)
{
	mutex.Lock();
	TimerList::iterator it = m_lstTickTimer.begin();
	for (; it != m_lstTickTimer.end(); it++)
		it->m_bPause = bPause;
	mutex.Unlock();

	TimerList::iterator iter = m_lstHandle.begin();
	for (; iter != m_lstHandle.end(); iter++)
		iter->m_bPause = bPause;
}

void CZTimer::Update(int nDelta)
{
	mutex.Lock();
	TimerList::iterator it = m_lstTickTimer.begin();
	for (; it != m_lstTickTimer.end(); it++)
	{
		//首先删除，然后增加
		TimerList::iterator iter = m_lstHandle.begin();
		for (; iter != m_lstHandle.end(); iter++)
		{
			if (iter->m_nType == it->m_nType)
			{
				m_lstHandle.erase(iter);
				break;
			}
		}
		if (!it->m_bToRemove)
			m_lstHandle.push_back(*it);
	}
	m_lstTickTimer.clear();
	mutex.Unlock();

	TimerList::iterator iter = m_lstHandle.begin();
	for (; iter != m_lstHandle.end(); iter++)
	{
		if (iter->m_bPause == false)
		{
			iter->m_nElapse += nDelta;

			if (iter->m_nElapse >= iter->m_nPeriod)
			{
				iter->m_nElapse -= iter->m_nPeriod;
				iter->OnTimerProc(iter->m_nType, iter->m_lpParam);
			}
		}
	}
}
