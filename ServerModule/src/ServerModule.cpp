#include "StdAfx.h"
#include "ServerModule.h"
#include "PlayerManage.h"
#include "CoreTable.h"
#include "BrokenRoom.h"
#include "RoomManage.h"
#include <INetServer.h>
#include "CorePlayerCreator.h"

static bool				 g_bFlagDestroy;
static bool				 g_bFlagMainLoop;
static HANDLE			 g_hThreadMainLoop;
CServerModule			*g_pServerModule = NULL;
INetServer				*g_pNetServer = NULL;
unsigned int			 g_uiCurTime = 0;

UINT WINAPI ServerMainLoop(LPVOID lpParameter)
{
	unsigned long endTick = GetTickCount();
	int nElapse;
	int nHeartBit = 0;

	for (;;)
	{
		if (g_bFlagDestroy)
		{
			g_bFlagMainLoop = false;
			return 0;
		}

		g_uiCurTime = GetTickCount();
		nElapse = g_uiCurTime - endTick;
		CBrokenRoom::GetInstancePtr()->OnLoop(nElapse); //断线是否超时
		g_pNetServer->OnLoop(nElapse); //服务器处理Q队列
		g_pZTimer->Update(nElapse); //更新定时器循环

		nHeartBit += nElapse;
		if (nHeartBit >= 1000 * 5)
		{
			RoomManage->HeartBit();
			nHeartBit = 0;
		}
		endTick = g_uiCurTime;
		::Sleep(10);
	}
	g_bFlagMainLoop = false;
	return 0;
}

bool InitServerMainLoop()
{
	unsigned long dwThreadIDMainLoop = 0;

	if (!g_hThreadMainLoop)
	{
		g_hThreadMainLoop = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerMainLoop, NULL, 0, &dwThreadIDMainLoop);

		if (g_hThreadMainLoop)
		{
			g_bFlagMainLoop = true;
			CloseHandle(g_hThreadMainLoop);
			g_hThreadMainLoop = NULL;
			return true;
		}
	}
	return false;
}

IServerModule* CreateServerModule()
{
	if (g_pServerModule == NULL)
	{
		g_pServerModule = new CServerModule();
	}
	return g_pServerModule;
}

CServerModule::CServerModule()
{
	g_pNetServer = CreateNetServer();
}

void CServerModule::InitServerModule(TServerConfig *pServerConfig)
{
	m_nPort			 = pServerConfig->nPort;
	m_nTotalRoom	 = pServerConfig->nTotalRoom;
	m_nTotalTable	 = pServerConfig->nTotalTable;
	m_nPlayerNum	 = pServerConfig->nPlayerNum;
	m_nGameMode		 = pServerConfig->nGameMode;
	m_nBattleMode	 = pServerConfig->nBattleMode;
	m_nStartMode	 = pServerConfig->nStartMode;
	m_bRunRandom	 = pServerConfig->bRunRandom;
	m_bRandomShuffle = pServerConfig->bRandomShuffle;

	g_pNetServer->InitNetServer(pServerConfig->nPort);
	g_pNetServer->SetCorePlayerCreator(CCorePlayerCreator::GetInstancePtr());

	RoomManage->Init(m_nTotalRoom);
	g_bFlagDestroy = false;
	g_bFlagMainLoop = false;
	InitServerMainLoop();
}

void CServerModule::Release()
{
	m_pTableCreator = NULL;

	//循环等待直到线程结束
	if (g_bFlagMainLoop)
	{
		g_bFlagDestroy = true;
		while (g_bFlagMainLoop);
	}

	//必须首先销毁房间资源
	RoomManage->Release();

	SAFE_RELEASE(g_pNetServer);
	delete this;
}

void CServerModule::SetTableCreator(ITableCreator* pTableCreator)
{
	m_pTableCreator = pTableCreator;
}
