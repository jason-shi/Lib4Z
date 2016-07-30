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

		//�������������߳�
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
	//ѭ���ȴ�ֱ���߳̽���
	if (g_bFlagUpdate)
	{
		g_bFlagDestroy = true;

		//�ڶ����߳��ͷ�֮ǰ����Ҫ����������Ϣ
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
	//��ֹ�ظ����
	DelAnime(pAnime);
	pAnime->m_pNext = m_pHead->m_pNext;
	m_pHead->m_pNext = pAnime;
}

void CZ2mov::DelAnime(CAnime *pAnime)
{
	CAnime *pNode = m_pHead;
	//����Ŀ�겢ɾ��
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
