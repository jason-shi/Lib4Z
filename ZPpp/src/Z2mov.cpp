#include "StdAfx.h"

static HANDLE		 g_hThreadUpdate = NULL;
static bool			 g_bFlagUpdate;
static bool			 g_bFlagDestroy;

UINT WINAPI ThreadUpdate(LPVOID lpParameter)
{
	int nStartTick = GetTickCount();
	while (!g_bFlagDestroy)
	{
		int nEndTick = GetTickCount();

		//动画播放在主线程
		g_pZ2coo->SendMessage(WM_ANIME_UPDATE, nEndTick - nStartTick);

		nStartTick = nEndTick;
		::Sleep(10);
	}
	g_bFlagUpdate = false;
	return 0;
}

bool InitThreadUpdate()
{
	if (!g_hThreadUpdate)
	{
		g_hThreadUpdate = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadUpdate, NULL, 0, NULL);
		if (g_hThreadUpdate)
		{
			CloseHandle(g_hThreadUpdate);
			g_hThreadUpdate = NULL;
			return true;
		}
	}
	return false;
}

CZ2mov::CZ2mov()
{
}

CZ2mov::~CZ2mov()
{
	if (m_pHead)
	{
		delete m_pHead;
		m_pHead = NULL;
	}
}

bool CZ2mov::Initial()
{
	m_pHead = new CAnime();

	g_bFlagUpdate = true;
	g_bFlagDestroy = false;

	return InitThreadUpdate();
}

void CZ2mov::Release()
{
	//循环等待直到线程结束
	if (g_bFlagUpdate)
	{
		g_bFlagDestroy = true;

		//在动画线程释放之前，还要继续处理消息
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

void CZ2mov::AddAnime(CAnime *pAnime)
{
	if (pAnime->IsActive())
		pAnime->Stop();
	//防止重复添加
	DelAnime(pAnime);
	pAnime->m_pNext = m_pHead->m_pNext;
	m_pHead->m_pNext = pAnime;
}

void CZ2mov::DelAnime(CAnime *pAnime)
{
	CAnime *pNode = m_pHead;
	//查找目标并删除
	while (pNode->m_pNext)
	{
		if (pNode->m_pNext == pAnime)
		{
			pNode->m_pNext = pAnime->m_pNext;
			pAnime->m_pNext = NULL;
			break;
		}
		pNode = pNode->m_pNext;
	}
}

void CZ2mov::Update(int nDelta)
{
	CAnime *pNode = m_pHead;
	while (pNode = pNode->m_pNext)
	{
		if (pNode->IsActive() && !pNode->IsPause())
			pNode->CAnime::Update(nDelta);
	}
}
