#include "StdAfx.h"
#include "TickTimer.h"

CTickTimer::CTickTimer(int nType, int nPeriod, FN_TimerProc TimerProc, void *lpParam, bool bToRemove)
{
	m_nType = nType;
	m_nPeriod = nPeriod;
	OnTimerProc = TimerProc;
	m_lpParam = lpParam;

	m_nElapse = 0;
	m_bPause = false;

	m_bToRemove = bToRemove;
}

CTickTimer::~CTickTimer()
{
}
